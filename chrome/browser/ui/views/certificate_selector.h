// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_CERTIFICATE_SELECTOR_H_
#define CHROME_BROWSER_UI_VIEWS_CERTIFICATE_SELECTOR_H_

#include <memory>

#include "base/macros.h"
#include "base/strings/string16.h"
#include "net/cert/x509_certificate.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/table/table_view_observer.h"
#include "ui/views/window/dialog_delegate.h"

namespace content {
class WebContents;
}

namespace views {
class LabelButton;
class TableView;
class View;
}

namespace ui {
class TableModel;
}

namespace chrome {

// A base class for dialogs that show a given list of certificates to the user.
// The user can select a single certificate and look at details of each
// certificate.
// The currently selected certificate can be obtained using |GetSelectedCert()|.
// The explanatory text shown to the user must be provided to |InitWithText()|.
class CertificateSelector : public views::DialogDelegateView,
                            public views::ButtonListener,
                            public views::TableViewObserver {
 public:
  // Indicates if the dialog can be successfully shown.
  // TODO(davidben): Remove this when the certificate selector prompt is moved
  // to the WebContentsDelegate. https://crbug.com/456255.
  static bool CanShow(content::WebContents* web_contents);

  // |web_contents| must not be null.
  CertificateSelector(const net::CertificateList& certificates,
                      content::WebContents* web_contents);
  ~CertificateSelector() override;

  // Returns the currently selected certificate or null if none is selected.
  // Must be called after |InitWithText()|.
  net::X509Certificate* GetSelectedCert() const;

  // Shows this dialog as a constrained web modal dialog and focuses the first
  // certificate.
  // Must be called after |InitWithText()|.
  void Show();

  // DialogDelegateView:
  bool CanResize() const override;
  base::string16 GetWindowTitle() const override;
  bool IsDialogButtonEnabled(ui::DialogButton button) const override;
  views::View* GetInitiallyFocusedView() override;
  views::View* CreateExtraView() override;
  ui::ModalType GetModalType() const override;

  // views::ButtonListener:
  void ButtonPressed(views::Button* sender, const ui::Event& event) override;

  // views::TableViewObserver:
  void OnSelectionChanged() override;
  void OnDoubleClick() override;

 protected:
  // The dimensions of the certificate selector table view, in pixels.
  static const int kTableViewWidth;
  static const int kTableViewHeight;

  // Initializes the dialog. |text| is shown above the list of certificates
  // and is supposed to explain to the user what the implication of the
  // certificate selection is.
  void InitWithText(std::unique_ptr<views::View> text_label);

  ui::TableModel* table_model_for_testing() const;

 private:
  class CertificateTableModel;

  net::CertificateList certificates_;

  // Whether to show the provider column in the table or not. Certificates
  // provided by the platform show the empty string as provider. That column is
  // shown only if there is at least one non-empty provider, i.e. non-platform
  // certificate.
  bool show_provider_column_ = false;
  std::unique_ptr<CertificateTableModel> model_;

  content::WebContents* const web_contents_;

  views::TableView* table_;
  views::LabelButton* view_cert_button_;

  DISALLOW_COPY_AND_ASSIGN(CertificateSelector);
};

}  // namespace chrome

#endif  // CHROME_BROWSER_UI_VIEWS_CERTIFICATE_SELECTOR_H_
