// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <algorithm>
#include <vector>

#include "base/files/file_enumerator.h"
#include "base/files/file_path.h"
#include "base/macros.h"
#include "base/path_service.h"
#include "base/strings/string_util.h"
#include "base/strings/utf_string_conversions.h"
#include "build/build_config.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/common/chrome_paths.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/test/base/ui_test_utils.h"
#include "components/autofill/content/browser/content_autofill_driver.h"
#include "components/autofill/content/browser/content_autofill_driver_factory.h"
#include "components/autofill/core/browser/autofill_manager.h"
#include "components/autofill/core/browser/data_driven_test.h"
#include "components/autofill/core/browser/form_structure.h"
#include "url/gurl.h"

#if defined(OS_MACOSX)
#include "base/mac/foundation_util.h"
#endif

namespace autofill {
namespace {

const base::FilePath::CharType kTestName[] = FILE_PATH_LITERAL("heuristics");

// Convert the |html| snippet to a data URI.
GURL HTMLToDataURI(const std::string& html) {
  // GURL requires data URLs to be UTF-8 and will fail below if it's not.
  CHECK(base::IsStringUTF8(html)) << "Input file is not UTF-8.";
  return GURL(std::string("data:text/html;charset=utf-8,") + html);
}

const base::FilePath& GetTestDataDir() {
  CR_DEFINE_STATIC_LOCAL(base::FilePath, dir, ());
  if (dir.empty())
    PathService::Get(chrome::DIR_TEST_DATA, &dir);
  return dir;
}

const std::vector<base::FilePath> GetTestFiles() {
  base::FilePath dir;
  CHECK(PathService::Get(base::DIR_SOURCE_ROOT, &dir));
  dir = dir.AppendASCII("chrome/test/data/autofill")
            .Append(kTestName)
            .AppendASCII("input");
  base::FileEnumerator input_files(dir, false, base::FileEnumerator::FILES);
  std::vector<base::FilePath> files;
  for (base::FilePath input_file = input_files.Next(); !input_file.empty();
       input_file = input_files.Next()) {
    files.push_back(input_file);
  }
  std::sort(files.begin(), files.end());

#if defined(OS_MACOSX)
  base::mac::ClearAmIBundledCache();
#endif  // defined(OS_MACOSX)

  return files;
}

}  // namespace

// A data-driven test for verifying Autofill heuristics. Each input is an HTML
// file that contains one or more forms. The corresponding output file lists the
// heuristically detected type for eachfield.
class FormStructureBrowserTest
    : public InProcessBrowserTest,
      public DataDrivenTest,
      public ::testing::WithParamInterface<base::FilePath> {
 protected:
  FormStructureBrowserTest();
  ~FormStructureBrowserTest() override;

  // DataDrivenTest:
  void GenerateResults(const std::string& input, std::string* output) override;

  // Serializes the given |forms| into a string.
  std::string FormStructuresToString(const std::vector<FormStructure*>& forms);

 private:
  DISALLOW_COPY_AND_ASSIGN(FormStructureBrowserTest);
};

FormStructureBrowserTest::FormStructureBrowserTest()
    : DataDrivenTest(GetTestDataDir()) {
}

FormStructureBrowserTest::~FormStructureBrowserTest() {
}

void FormStructureBrowserTest::GenerateResults(const std::string& input,
                                               std::string* output) {
  ASSERT_NO_FATAL_FAILURE(ui_test_utils::NavigateToURL(browser(),
                                                       HTMLToDataURI(input)));

  content::WebContents* web_contents =
      browser()->tab_strip_model()->GetActiveWebContents();
  ContentAutofillDriver* autofill_driver =
      ContentAutofillDriverFactory::FromWebContents(web_contents)
          ->DriverForFrame(web_contents->GetMainFrame());
  ASSERT_NE(nullptr, autofill_driver);
  AutofillManager* autofill_manager = autofill_driver->autofill_manager();
  ASSERT_NE(nullptr, autofill_manager);
  std::vector<FormStructure*> forms = autofill_manager->form_structures_.get();
  *output = FormStructuresToString(forms);
}

std::string FormStructureBrowserTest::FormStructuresToString(
    const std::vector<FormStructure*>& forms) {
  std::string forms_string;
  for (const FormStructure* form : forms) {
    for (const AutofillField* field : *form) {
      forms_string += field->Type().ToString();
      forms_string += " | " + base::UTF16ToUTF8(field->name);
      forms_string += " | " + base::UTF16ToUTF8(field->label);
      forms_string += " | " + base::UTF16ToUTF8(field->value);
      forms_string += " | " + field->section();
      forms_string += "\n";
    }
  }
  return forms_string;
}

IN_PROC_BROWSER_TEST_P(FormStructureBrowserTest, DataDrivenHeuristics) {
  // Prints the path of the test to be executed.
  LOG(INFO) << GetParam().MaybeAsASCII();
  RunOneDataDrivenTest(GetParam(), GetOutputDirectory(kTestName));
}

INSTANTIATE_TEST_CASE_P(AllForms,
                        FormStructureBrowserTest,
                        testing::ValuesIn(GetTestFiles()));

}  // namespace autofill
