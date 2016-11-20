// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/page_load_metrics/observers/from_gws_page_load_metrics_observer.h"

#include "base/macros.h"
#include "base/memory/ptr_util.h"
#include "chrome/browser/page_load_metrics/observers/page_load_metrics_observer_test_harness.h"
#include "components/page_load_metrics/browser/page_load_metrics_util.h"

namespace {
const char kExampleUrl[] = "http://www.example.com/";
const char kGoogleSearchResultsUrl[] = "https://www.google.com/webhp?q=d";
}  // namespace

class FromGWSPageLoadMetricsObserverTest
    : public page_load_metrics::PageLoadMetricsObserverTestHarness {
 public:
  void RegisterObservers(page_load_metrics::PageLoadTracker* tracker) override {
    FromGWSPageLoadMetricsObserver* observer =
        new FromGWSPageLoadMetricsObserver();
    logger_ = observer->GetLogger();
    tracker->AddObserver(base::WrapUnique(observer));
  }

  void SimulateTimingWithoutPaint() {
    page_load_metrics::PageLoadTiming timing;
    timing.navigation_start = base::Time::FromDoubleT(1);
    SimulateTimingUpdate(timing);
  }

  void SimulateTimingWithFirstPaint() {
    page_load_metrics::PageLoadTiming timing;
    timing.navigation_start = base::Time::FromDoubleT(1);
    // Use 0 and invoke OnFirstPaint here as a hack for current time_to_abort
    // in cases in release env are always 0
    // TODO(bmcquade): replace it by 1
    timing.first_paint = base::TimeDelta::FromMilliseconds(0);
    PopulateRequiredTimingFields(&timing);
    SimulateTimingUpdate(timing);
    // TODO(bmcquade): remove SetFirstPaintTriggered as part of fixing
    // crbug.com/616901
    logger_->SetFirstPaintTriggered(true);
  }

  void SimulateMouseEvent() {
    blink::WebMouseEvent mouse_event;
    mouse_event.type = blink::WebInputEvent::MouseDown;
    mouse_event.button = blink::WebMouseEvent::ButtonLeft;
    mouse_event.x = 7;
    mouse_event.y = 7;
    mouse_event.clickCount = 1;
    SimulateInputEvent(mouse_event);
  }

 protected:
  // TODO(bmcquade): remove once crbug.com/616901 is addressed
  FromGWSPageLoadMetricsLogger* logger_;
};

class FromGWSPageLoadMetricsLoggerTest : public testing::Test {};

TEST_F(FromGWSPageLoadMetricsObserverTest, NoMetrics) {
  histogram_tester().ExpectTotalCount(internal::kHistogramFromGWSFirstTextPaint,
                                      0);
}

TEST_F(FromGWSPageLoadMetricsObserverTest, NoPreviousCommittedUrl) {
  page_load_metrics::PageLoadTiming timing;
  timing.navigation_start = base::Time::FromDoubleT(1);
  timing.first_text_paint = base::TimeDelta::FromMilliseconds(1);
  PopulateRequiredTimingFields(&timing);
  NavigateAndCommit(GURL(kExampleUrl));

  SimulateTimingUpdate(timing);

  // Navigate again to force logging.
  NavigateAndCommit(GURL("http://www.final.com"));
  histogram_tester().ExpectTotalCount(internal::kHistogramFromGWSFirstTextPaint,
                                      0);
}

TEST_F(FromGWSPageLoadMetricsObserverTest, NonSearchPreviousCommittedUrl) {
  page_load_metrics::PageLoadTiming timing;
  timing.navigation_start = base::Time::FromDoubleT(1);
  timing.first_text_paint = base::TimeDelta::FromMilliseconds(1);
  PopulateRequiredTimingFields(&timing);
  NavigateAndCommit(GURL("http://www.other.com"));
  NavigateAndCommit(GURL(kExampleUrl));

  SimulateTimingUpdate(timing);

  // Navigate again to force logging.
  NavigateAndCommit(GURL("http://www.final.com"));
  histogram_tester().ExpectTotalCount(internal::kHistogramFromGWSFirstTextPaint,
                                      0);
}

TEST_F(FromGWSPageLoadMetricsObserverTest,
       GoogleNonSearchPreviousCommittedUrl1) {
  page_load_metrics::PageLoadTiming timing;
  timing.navigation_start = base::Time::FromDoubleT(1);
  timing.first_text_paint = base::TimeDelta::FromMilliseconds(1);
  PopulateRequiredTimingFields(&timing);
  NavigateAndCommit(GURL("https://www.google.com/"));
  NavigateAndCommit(GURL(kExampleUrl));

  SimulateTimingUpdate(timing);

  // Navigate again to force logging.
  NavigateAndCommit(GURL("http://www.final.com"));
  histogram_tester().ExpectTotalCount(internal::kHistogramFromGWSFirstTextPaint,
                                      0);
}

TEST_F(FromGWSPageLoadMetricsObserverTest,
       GoogleNonSearchPreviousCommittedUrl2) {
  page_load_metrics::PageLoadTiming timing;
  timing.navigation_start = base::Time::FromDoubleT(1);
  timing.first_text_paint = base::TimeDelta::FromMilliseconds(1);
  PopulateRequiredTimingFields(&timing);
  // Navigation from /search, but missing a query string, so can't have been a
  // search results page.
  NavigateAndCommit(GURL("https://www.google.com/search?a=b&c=d"));
  NavigateAndCommit(GURL(kExampleUrl));

  SimulateTimingUpdate(timing);

  // Navigate again to force logging.
  NavigateAndCommit(GURL("http://www.final.com"));
  histogram_tester().ExpectTotalCount(internal::kHistogramFromGWSFirstTextPaint,
                                      0);
}

TEST_F(FromGWSPageLoadMetricsObserverTest, SearchPreviousCommittedUrl1) {
  page_load_metrics::PageLoadTiming timing;
  timing.navigation_start = base::Time::FromDoubleT(1);
  timing.parse_start = base::TimeDelta::FromMilliseconds(10);
  timing.first_paint = base::TimeDelta::FromMilliseconds(20);
  timing.first_contentful_paint = base::TimeDelta::FromMilliseconds(40);
  timing.first_text_paint = base::TimeDelta::FromMilliseconds(80);
  timing.first_image_paint = base::TimeDelta::FromMilliseconds(160);
  timing.parse_stop = base::TimeDelta::FromMilliseconds(320);
  timing.dom_content_loaded_event_start =
      base::TimeDelta::FromMilliseconds(640);
  timing.load_event_start = base::TimeDelta::FromMilliseconds(1280);
  PopulateRequiredTimingFields(&timing);
  NavigateAndCommit(GURL("https://www.google.com/webhp?q=test"));
  NavigateAndCommit(GURL(kExampleUrl));

  SimulateTimingUpdate(timing);

  // Navigate again to force logging.
  NavigateAndCommit(GURL("http://www.final.com"));

  histogram_tester().ExpectTotalCount(internal::kHistogramFromGWSParseStart, 1);
  histogram_tester().ExpectBucketCount(internal::kHistogramFromGWSParseStart,
                                       timing.parse_start.InMilliseconds(), 1);

  histogram_tester().ExpectTotalCount(internal::kHistogramFromGWSFirstPaint, 1);
  histogram_tester().ExpectBucketCount(internal::kHistogramFromGWSFirstPaint,
                                       timing.first_paint.InMilliseconds(), 1);

  histogram_tester().ExpectTotalCount(
      internal::kHistogramFromGWSFirstContentfulPaint, 1);
  histogram_tester().ExpectBucketCount(
      internal::kHistogramFromGWSFirstContentfulPaint,
      timing.first_contentful_paint.InMilliseconds(), 1);

  histogram_tester().ExpectTotalCount(
      internal::kHistogramFromGWSParseStartToFirstContentfulPaint, 1);
  histogram_tester().ExpectBucketCount(
      internal::kHistogramFromGWSParseStartToFirstContentfulPaint,
      (timing.first_contentful_paint - timing.parse_start).InMilliseconds(), 1);

  histogram_tester().ExpectTotalCount(internal::kHistogramFromGWSFirstTextPaint,
                                      1);
  histogram_tester().ExpectBucketCount(
      internal::kHistogramFromGWSFirstTextPaint,
      timing.first_text_paint.InMilliseconds(), 1);

  histogram_tester().ExpectTotalCount(
      internal::kHistogramFromGWSFirstImagePaint, 1);
  histogram_tester().ExpectBucketCount(
      internal::kHistogramFromGWSFirstImagePaint,
      timing.first_image_paint.InMilliseconds(), 1);

  histogram_tester().ExpectTotalCount(internal::kHistogramFromGWSParseDuration,
                                      1);
  histogram_tester().ExpectBucketCount(
      internal::kHistogramFromGWSParseDuration,
      (timing.parse_stop - timing.parse_start).InMilliseconds(), 1);

  histogram_tester().ExpectTotalCount(
      internal::kHistogramFromGWSDomContentLoaded, 1);
  histogram_tester().ExpectBucketCount(
      internal::kHistogramFromGWSDomContentLoaded,
      timing.dom_content_loaded_event_start.InMilliseconds(), 1);

  histogram_tester().ExpectTotalCount(internal::kHistogramFromGWSLoad, 1);
  histogram_tester().ExpectBucketCount(internal::kHistogramFromGWSLoad,
                                       timing.load_event_start.InMilliseconds(),
                                       1);
}

TEST_F(FromGWSPageLoadMetricsObserverTest, SearchPreviousCommittedUrl2) {
  page_load_metrics::PageLoadTiming timing;
  timing.navigation_start = base::Time::FromDoubleT(1);
  timing.first_text_paint = base::TimeDelta::FromMilliseconds(1);
  PopulateRequiredTimingFields(&timing);
  NavigateAndCommit(GURL("https://www.google.com/#q=test"));
  NavigateAndCommit(GURL(kExampleUrl));

  SimulateTimingUpdate(timing);

  // Navigate again to force logging.
  NavigateAndCommit(GURL("http://www.final.com"));
  histogram_tester().ExpectTotalCount(internal::kHistogramFromGWSFirstTextPaint,
                                      1);
  histogram_tester().ExpectBucketCount(
      internal::kHistogramFromGWSFirstTextPaint,
      timing.first_text_paint.InMilliseconds(), 1);
}

TEST_F(FromGWSPageLoadMetricsObserverTest, SearchPreviousCommittedUrl3) {
  page_load_metrics::PageLoadTiming timing;
  timing.navigation_start = base::Time::FromDoubleT(1);
  timing.first_text_paint = base::TimeDelta::FromMilliseconds(1);
  PopulateRequiredTimingFields(&timing);
  NavigateAndCommit(GURL("https://www.google.com/webhp#q=test"));
  NavigateAndCommit(GURL(kExampleUrl));

  SimulateTimingUpdate(timing);

  // Navigate again to force logging.
  NavigateAndCommit(GURL("http://www.final.com"));
  histogram_tester().ExpectTotalCount(internal::kHistogramFromGWSFirstTextPaint,
                                      1);
  histogram_tester().ExpectBucketCount(
      internal::kHistogramFromGWSFirstTextPaint,
      timing.first_text_paint.InMilliseconds(), 1);
}

TEST_F(FromGWSPageLoadMetricsObserverTest, SearchPreviousCommittedUrl4) {
  page_load_metrics::PageLoadTiming timing;
  timing.navigation_start = base::Time::FromDoubleT(1);
  timing.first_text_paint = base::TimeDelta::FromMilliseconds(1);
  PopulateRequiredTimingFields(&timing);
  NavigateAndCommit(GURL("https://www.google.co.uk/search#q=test"));
  NavigateAndCommit(GURL(kExampleUrl));

  SimulateTimingUpdate(timing);

  // Navigate again to force logging.
  NavigateAndCommit(GURL("http://www.final.com"));
  histogram_tester().ExpectTotalCount(internal::kHistogramFromGWSFirstTextPaint,
                                      1);
  histogram_tester().ExpectBucketCount(
      internal::kHistogramFromGWSFirstTextPaint,
      timing.first_text_paint.InMilliseconds(), 1);
}

TEST_F(FromGWSPageLoadMetricsObserverTest, SearchToNonSearchToOtherPage) {
  page_load_metrics::PageLoadTiming timing;
  timing.navigation_start = base::Time::FromDoubleT(1);
  timing.first_text_paint = base::TimeDelta::FromMilliseconds(1);
  page_load_metrics::PageLoadTiming timing2;
  timing2.navigation_start = base::Time::FromDoubleT(2);
  timing2.first_text_paint = base::TimeDelta::FromMilliseconds(100);
  PopulateRequiredTimingFields(&timing);
  PopulateRequiredTimingFields(&timing2);
  NavigateAndCommit(GURL("https://www.google.co.uk/search#q=test"));
  NavigateAndCommit(GURL(kExampleUrl));
  SimulateTimingUpdate(timing);
  NavigateAndCommit(GURL("http://www.example.com/other"));
  SimulateTimingUpdate(timing2);

  // Navigate again to force logging. We expect to log timing for the page
  // navigated from search, but not for the page navigated from that page.
  NavigateAndCommit(GURL("http://www.final.com"));
  histogram_tester().ExpectTotalCount(internal::kHistogramFromGWSFirstTextPaint,
                                      1);
  histogram_tester().ExpectBucketCount(
      internal::kHistogramFromGWSFirstTextPaint,
      timing.first_text_paint.InMilliseconds(), 1);
}

TEST_F(FromGWSPageLoadMetricsObserverTest, SearchToNonSearchToSearch) {
  page_load_metrics::PageLoadTiming timing;
  timing.navigation_start = base::Time::FromDoubleT(1);
  timing.first_text_paint = base::TimeDelta::FromMilliseconds(1);
  page_load_metrics::PageLoadTiming timing2;
  timing2.navigation_start = base::Time::FromDoubleT(2);
  timing2.first_text_paint = base::TimeDelta::FromMilliseconds(100);
  PopulateRequiredTimingFields(&timing);
  PopulateRequiredTimingFields(&timing2);
  NavigateAndCommit(GURL("https://www.google.co.uk/search#q=test"));
  NavigateAndCommit(GURL(kExampleUrl));
  SimulateTimingUpdate(timing);
  NavigateAndCommit(GURL("https://www.google.co.uk/search#q=test"));
  SimulateTimingUpdate(timing2);

  // Navigate again to force logging. We expect to log timing for the page
  // navigated from search, but not for the search page we navigated to.
  NavigateAndCommit(GURL("http://www.final.com"));
  histogram_tester().ExpectTotalCount(internal::kHistogramFromGWSFirstTextPaint,
                                      1);
  histogram_tester().ExpectBucketCount(
      internal::kHistogramFromGWSFirstTextPaint,
      timing.first_text_paint.InMilliseconds(), 1);
}

TEST_F(FromGWSPageLoadMetricsObserverTest,
       SearchToNonSearchToSearchToNonSearch) {
  page_load_metrics::PageLoadTiming timing;
  timing.navigation_start = base::Time::FromDoubleT(1);
  timing.first_text_paint = base::TimeDelta::FromMilliseconds(1);
  page_load_metrics::PageLoadTiming timing2;
  timing2.navigation_start = base::Time::FromDoubleT(2);
  timing2.first_text_paint = base::TimeDelta::FromMilliseconds(100);
  page_load_metrics::PageLoadTiming timing3;
  timing3.navigation_start = base::Time::FromDoubleT(3);
  timing3.first_text_paint = base::TimeDelta::FromMilliseconds(1000);
  PopulateRequiredTimingFields(&timing);
  PopulateRequiredTimingFields(&timing2);
  PopulateRequiredTimingFields(&timing3);
  NavigateAndCommit(GURL("https://www.google.co.uk/search#q=test"));
  NavigateAndCommit(GURL(kExampleUrl));
  SimulateTimingUpdate(timing);
  NavigateAndCommit(GURL("https://www.google.co.uk/search#q=test"));
  SimulateTimingUpdate(timing2);
  NavigateAndCommit(GURL(kExampleUrl));
  SimulateTimingUpdate(timing3);

  // Navigate again to force logging. We expect to log timing for both pages
  // navigated from search, but not for the search pages we navigated to.
  NavigateAndCommit(GURL("http://www.final.com"));
  histogram_tester().ExpectTotalCount(internal::kHistogramFromGWSFirstTextPaint,
                                      2);
  histogram_tester().ExpectBucketCount(
      internal::kHistogramFromGWSFirstTextPaint,
      timing.first_text_paint.InMilliseconds(), 1);
  histogram_tester().ExpectBucketCount(
      internal::kHistogramFromGWSFirstTextPaint,
      timing3.first_text_paint.InMilliseconds(), 1);
}

TEST_F(FromGWSPageLoadMetricsObserverTest,
       SearchToNonSearchToSearchToNonSearchBackgrounded) {
  page_load_metrics::PageLoadTiming timing;
  timing.navigation_start = base::Time::FromDoubleT(1);
  timing.first_text_paint = base::TimeDelta::FromMilliseconds(1);
  page_load_metrics::PageLoadTiming timing2;
  timing2.navigation_start = base::Time::FromDoubleT(2);
  timing2.first_text_paint = base::TimeDelta::FromMilliseconds(100);
  page_load_metrics::PageLoadTiming timing3;
  timing3.navigation_start = base::Time::FromDoubleT(3);
  timing3.first_text_paint = base::TimeDelta::FromMilliseconds(1000);
  PopulateRequiredTimingFields(&timing);
  PopulateRequiredTimingFields(&timing2);
  PopulateRequiredTimingFields(&timing3);
  NavigateAndCommit(GURL("https://www.google.co.uk/search#q=test"));
  NavigateAndCommit(GURL(kExampleUrl));
  SimulateTimingUpdate(timing);
  NavigateAndCommit(GURL("https://www.google.co.uk/search#q=test"));
  web_contents()->WasHidden();
  SimulateTimingUpdate(timing2);
  NavigateAndCommit(GURL(kExampleUrl));
  SimulateTimingUpdate(timing3);

  // Navigate again to force logging. We expect to log timing for the first page
  // navigated from search, but not the second since it was backgrounded.
  NavigateAndCommit(GURL("http://www.final.com"));
  histogram_tester().ExpectTotalCount(internal::kHistogramFromGWSFirstTextPaint,
                                      1);
  histogram_tester().ExpectBucketCount(
      internal::kHistogramFromGWSFirstTextPaint,
      timing.first_text_paint.InMilliseconds(), 1);
}

TEST_F(FromGWSPageLoadMetricsObserverTest,
       SearchRedirectorPreviousCommittedUrl) {
  page_load_metrics::PageLoadTiming timing;
  timing.navigation_start = base::Time::FromDoubleT(1);
  timing.first_text_paint = base::TimeDelta::FromMilliseconds(1);
  PopulateRequiredTimingFields(&timing);
  NavigateAndCommit(GURL("https://www.google.com/search#q=test"));
  NavigateAndCommit(GURL("https://www.google.com/url?source=web"));
  NavigateAndCommit(GURL(kExampleUrl));

  SimulateTimingUpdate(timing);

  // Navigate again to force logging.
  NavigateAndCommit(GURL("http://www.final.com"));
  histogram_tester().ExpectTotalCount(internal::kHistogramFromGWSFirstTextPaint,
                                      1);
  histogram_tester().ExpectBucketCount(
      internal::kHistogramFromGWSFirstTextPaint,
      timing.first_text_paint.InMilliseconds(), 1);
}

TEST_F(FromGWSPageLoadMetricsObserverTest,
       NonSearchRedirectorPreviousCommittedUrl) {
  page_load_metrics::PageLoadTiming timing;
  timing.navigation_start = base::Time::FromDoubleT(1);
  timing.first_text_paint = base::TimeDelta::FromMilliseconds(1);
  PopulateRequiredTimingFields(&timing);
  NavigateAndCommit(GURL("https://www.google.com/webhp?q=test"));
  NavigateAndCommit(GURL("https://www.google.com/url?a=b&c=d"));
  NavigateAndCommit(GURL(kExampleUrl));

  SimulateTimingUpdate(timing);

  // Navigate again to force logging.
  NavigateAndCommit(GURL("http://www.final.com"));
  histogram_tester().ExpectTotalCount(internal::kHistogramFromGWSFirstTextPaint,
                                      0);
}

TEST_F(FromGWSPageLoadMetricsObserverTest,
       SearchPreviousCommittedUrlBackgroundLater) {
  page_load_metrics::PageLoadTiming timing;
  timing.navigation_start = base::Time::FromDoubleT(1);
  timing.first_text_paint = base::TimeDelta::FromMicroseconds(1);
  PopulateRequiredTimingFields(&timing);

  NavigateAndCommit(GURL("https://www.google.com/search#q=test"));
  NavigateAndCommit(GURL(kExampleUrl));

  web_contents()->WasHidden();
  SimulateTimingUpdate(timing);

  page_load_metrics::PageLoadExtraInfo info =
      GetPageLoadExtraInfoForCommittedLoad();

  // If the system clock is low resolution PageLoadTracker's background_time_
  // may be < timing.first_text_paint.
  if (page_load_metrics::WasStartedInForegroundEventInForeground(
          timing.first_text_paint, info)) {
    histogram_tester().ExpectTotalCount(
        internal::kHistogramFromGWSFirstTextPaint, 1);
    histogram_tester().ExpectBucketCount(
        internal::kHistogramFromGWSFirstTextPaint,
        timing.first_text_paint.InMilliseconds(), 1);
  } else {
    histogram_tester().ExpectTotalCount(
        internal::kHistogramFromGWSFirstTextPaint, 0);
  }
}

TEST_F(FromGWSPageLoadMetricsObserverTest, UnknownNavigationBeforeCommit) {
  NavigateAndCommit(GURL(kGoogleSearchResultsUrl));
  StartNavigation(GURL("http://example.test"));

  // Simulate the user performing another navigation before commit.
  StartNavigation(GURL("https://www.example.com"));
  histogram_tester().ExpectTotalCount(
      internal::kHistogramFromGWSAbortUnknownNavigationBeforeCommit, 1);
}

TEST_F(FromGWSPageLoadMetricsObserverTest, NewNavigationBeforePaint) {
  NavigateAndCommit(GURL(kGoogleSearchResultsUrl));
  NavigateAndCommit(GURL("http://example.test"));
  SimulateTimingWithoutPaint();
  // Simulate the user performing another navigation before paint.
  NavigateAndCommit(GURL("https://www.example.com"));
  histogram_tester().ExpectTotalCount(
      internal::kHistogramFromGWSAbortNewNavigationBeforePaint, 1);
}

TEST_F(FromGWSPageLoadMetricsObserverTest, StopBeforeCommit) {
  NavigateAndCommit(GURL(kGoogleSearchResultsUrl));
  StartNavigation(GURL("http://example.test"));
  // Simulate the user pressing the stop button.
  web_contents()->Stop();
  // Now close the tab. This will trigger logging for the prior navigation which
  // was stopped above.
  DeleteContents();
  histogram_tester().ExpectTotalCount(
      internal::kHistogramFromGWSAbortStopBeforeCommit, 1);
}

TEST_F(FromGWSPageLoadMetricsObserverTest, StopBeforeCommitNonSearch) {
  NavigateAndCommit(GURL("http://google.com"));
  StartNavigation(GURL("http://example.test"));
  // Simulate the user pressing the stop button.
  web_contents()->Stop();
  // Now close the tab. This will trigger logging for the prior navigation which
  // was stopped above.
  DeleteContents();
  histogram_tester().ExpectTotalCount(
      internal::kHistogramFromGWSAbortStopBeforeCommit, 0);
}

TEST_F(FromGWSPageLoadMetricsObserverTest, StopBeforeCommitSearchToSearch) {
  NavigateAndCommit(GURL(kGoogleSearchResultsUrl));
  StartNavigation(GURL("http://www.google.com/webhp?q=5"));
  // Simulate the user pressing the stop button.
  web_contents()->Stop();
  // Now close the tab. This will trigger logging for the prior navigation which
  // was stopped above.
  DeleteContents();
  histogram_tester().ExpectTotalCount(
      internal::kHistogramFromGWSAbortStopBeforeCommit, 0);
}

TEST_F(FromGWSPageLoadMetricsObserverTest, StopBeforePaint) {
  NavigateAndCommit(GURL(kGoogleSearchResultsUrl));
  NavigateAndCommit(GURL("http://example.test"));
  SimulateTimingWithoutPaint();
  // Simulate the user pressing the stop button.
  web_contents()->Stop();
  // Now close the tab. This will trigger logging for the prior navigation which
  // was stopped above.
  DeleteContents();
  histogram_tester().ExpectTotalCount(
      internal::kHistogramFromGWSAbortStopBeforePaint, 1);
}

TEST_F(FromGWSPageLoadMetricsObserverTest, StopBeforeCommitAndBeforePaint) {
  // Commit the first navigation.
  NavigateAndCommit(GURL(kGoogleSearchResultsUrl));
  NavigateAndCommit(GURL("https://example.test"));
  SimulateTimingWithoutPaint();
  // Now start a second navigation, but don't commit it.
  StartNavigation(GURL("https://www.google.com"));
  // Simulate the user pressing the stop button. This should cause us to record
  // stop metrics for the FromGWS committed load, too.
  web_contents()->Stop();
  // Simulate closing the tab.
  DeleteContents();
  // The second navigation was not from GWS.
  histogram_tester().ExpectTotalCount(
      internal::kHistogramFromGWSAbortStopBeforeCommit, 0);
  histogram_tester().ExpectTotalCount(
      internal::kHistogramFromGWSAbortStopBeforePaint, 1);
}

TEST_F(FromGWSPageLoadMetricsObserverTest, CloseBeforeCommit) {
  NavigateAndCommit(GURL(kGoogleSearchResultsUrl));
  StartNavigation(GURL("https://example.test"));
  // Simulate closing the tab.
  DeleteContents();
  histogram_tester().ExpectTotalCount(
      internal::kHistogramFromGWSAbortCloseBeforeCommit, 1);
}

TEST_F(FromGWSPageLoadMetricsObserverTest, CloseBeforePaint) {
  NavigateAndCommit(GURL(kGoogleSearchResultsUrl));
  NavigateAndCommit(GURL("https://example.test"));
  SimulateTimingWithoutPaint();
  // Simulate closing the tab.
  DeleteContents();
  histogram_tester().ExpectTotalCount(
      internal::kHistogramFromGWSAbortCloseBeforePaint, 1);
}

TEST_F(FromGWSPageLoadMetricsObserverTest,
       AbortCloseBeforeCommitAndBeforePaint) {
  // Commit the first navigation.
  NavigateAndCommit(GURL(kGoogleSearchResultsUrl));
  NavigateAndCommit(GURL("https://example.test"));
  SimulateTimingWithoutPaint();
  // Now start a second navigation, but don't commit it.
  StartNavigation(GURL("https://example.test2"));
  // Simulate closing the tab.
  DeleteContents();
  // The second navigation was not from GWS.
  histogram_tester().ExpectTotalCount(
      internal::kHistogramFromGWSAbortCloseBeforeCommit, 0);
  histogram_tester().ExpectTotalCount(
      internal::kHistogramFromGWSAbortCloseBeforePaint, 1);
}

TEST_F(FromGWSPageLoadMetricsObserverTest,
       AbortStopBeforeCommitAndCloseBeforePaint) {
  NavigateAndCommit(GURL(kGoogleSearchResultsUrl));
  StartNavigation(GURL("https://example.test"));
  // Simulate the user pressing the stop button.
  web_contents()->Stop();
  NavigateAndCommit(GURL("https://example.test2"));
  SimulateTimingWithoutPaint();
  // Simulate closing the tab.
  DeleteContents();
  histogram_tester().ExpectTotalCount(
      internal::kHistogramFromGWSAbortStopBeforeCommit, 1);
  // The second navigation was from GWS, as GWS was the last committed URL.
  histogram_tester().ExpectTotalCount(
      internal::kHistogramFromGWSAbortCloseBeforePaint, 1);
}

// TODO(bmcquade, csharrison): add tests for reload, back/forward, and other
// aborts.

TEST_F(FromGWSPageLoadMetricsObserverTest, NoAbortNewNavigationFromAboutURL) {
  NavigateAndCommit(GURL(kGoogleSearchResultsUrl));
  NavigateAndCommit(GURL("about:blank"));
  NavigateAndCommit(GURL("https://www.example.com"));
  histogram_tester().ExpectTotalCount(
      internal::kHistogramFromGWSAbortNewNavigationBeforePaint, 0);
}

TEST_F(FromGWSPageLoadMetricsObserverTest, NoAbortNewNavigationAfterPaint) {
  NavigateAndCommit(GURL(kGoogleSearchResultsUrl));
  page_load_metrics::PageLoadTiming timing;
  timing.navigation_start = base::Time::FromDoubleT(1);
  timing.first_paint = base::TimeDelta::FromMicroseconds(1);
  PopulateRequiredTimingFields(&timing);
  NavigateAndCommit(GURL("https://example.test"));
  SimulateTimingUpdate(timing);

  // The test cannot assume that abort time will be > first_paint
  // (1 micro-sec). If the system clock is low resolution, PageLoadTracker's
  // abort time may be <= first_paint. In that case the histogram will be
  // logged. Thus both 0 and 1 counts of histograms are considered good.

  NavigateAndCommit(GURL("https://example.test2"));

  base::HistogramTester::CountsMap counts_map =
      histogram_tester().GetTotalCountsForPrefix(
          internal::kHistogramFromGWSAbortNewNavigationBeforePaint);

  EXPECT_TRUE(counts_map.empty() ||
              (counts_map.size() == 1 && counts_map.begin()->second == 1));
}

TEST_F(FromGWSPageLoadMetricsObserverTest, NewNavigationBeforeInteraction) {
  NavigateAndCommit(GURL(kGoogleSearchResultsUrl));
  NavigateAndCommit(GURL("http://example.test"));
  SimulateTimingWithFirstPaint();
  // Simulate the user performing another navigation before paint.
  NavigateAndCommit(GURL("https://www.example.com"));
  histogram_tester().ExpectTotalCount(
      internal::kHistogramFromGWSAbortNewNavigationBeforeInteraction, 1);
}

TEST_F(FromGWSPageLoadMetricsObserverTest, StopBeforeInteraction) {
  NavigateAndCommit(GURL(kGoogleSearchResultsUrl));
  NavigateAndCommit(GURL("http://example.test"));
  SimulateTimingWithFirstPaint();
  // Simulate the user pressing the stop button.
  web_contents()->Stop();
  // Now close the tab. This will trigger logging for the prior navigation which
  // was stopped above.
  DeleteContents();
  histogram_tester().ExpectTotalCount(
      internal::kHistogramFromGWSAbortStopBeforeInteraction, 1);
}

TEST_F(FromGWSPageLoadMetricsObserverTest, CloseBeforeInteraction) {
  NavigateAndCommit(GURL(kGoogleSearchResultsUrl));
  NavigateAndCommit(GURL("https://example.test"));
  SimulateTimingWithFirstPaint();
  // Simulate closing the tab.
  DeleteContents();
  histogram_tester().ExpectTotalCount(
      internal::kHistogramFromGWSAbortCloseBeforeInteraction, 1);
}

TEST_F(FromGWSPageLoadMetricsObserverTest, CloseBeforePaintAndInteraction) {
  NavigateAndCommit(GURL(kGoogleSearchResultsUrl));
  NavigateAndCommit(GURL("https://example.test"));
  SimulateTimingWithoutPaint();
  // Simulate closing the tab.
  DeleteContents();
  histogram_tester().ExpectTotalCount(
      internal::kHistogramFromGWSAbortCloseBeforeInteraction, 0);
}

TEST_F(FromGWSPageLoadMetricsObserverTest, CloseAfterInteraction) {
  NavigateAndCommit(GURL(kGoogleSearchResultsUrl));
  NavigateAndCommit(GURL("https://example.test"));
  SimulateTimingWithFirstPaint();
  // Simulate user interaction.
  SimulateMouseEvent();
  // Simulate closing the tab.
  DeleteContents();
  histogram_tester().ExpectTotalCount(
      internal::kHistogramFromGWSAbortCloseBeforeInteraction, 0);
}

TEST_F(FromGWSPageLoadMetricsLoggerTest, IsGoogleSearchHostname) {
  struct {
    bool expected_result;
    const char* url;
  } test_cases[] = {
      {true, "www.google.com"},
      {true, "www.google.co.uk"},
      {true, "www.google.co.in"},
      {false, "other.google.com"},
      {false, "other.www.google.com"},
      {false, "www.other.google.com"},
      {false, "www.www.google.com"},
      {false, "www.google.appspot.com"},
      {false, "www.google.example.com"},
      // Search results are not served from the bare google.com domain.
      {false, "google.com"},
  };
  for (const auto& test : test_cases) {
    EXPECT_EQ(test.expected_result,
              FromGWSPageLoadMetricsLogger::IsGoogleSearchHostname(test.url))
        << "for URL: " << test.url;
  }
}

TEST_F(FromGWSPageLoadMetricsLoggerTest, IsGoogleSearchResultUrl) {
  struct {
    bool expected_result;
    const char* url;
  } test_cases[] = {
      {true, "https://www.google.com/#q=test"},
      {true, "https://www.google.com/search#q=test"},
      {true, "https://www.google.com/search?q=test"},
      {true, "https://www.google.com/webhp#q=test"},
      {true, "https://www.google.com/webhp?q=test"},
      {true, "https://www.google.com/webhp?a=b&q=test"},
      {true, "https://www.google.com/webhp?a=b&q=test&c=d"},
      {true, "https://www.google.com/webhp#a=b&q=test&c=d"},
      {true, "https://www.google.com/webhp?#a=b&q=test&c=d"},
      {false, "https://www.google.com/"},
      {false, "https://www.google.com/about/"},
      {false, "https://other.google.com/"},
      {false, "https://other.google.com/webhp?q=test"},
      {false, kExampleUrl},
      {false, "https://www.example.com/webhp?q=test"},
      {false, "https://google.com/#q=test"},
  };
  for (const auto& test : test_cases) {
    EXPECT_EQ(
        test.expected_result,
        FromGWSPageLoadMetricsLogger::IsGoogleSearchResultUrl(GURL(test.url)))
        << "for URL: " << test.url;
  }
}

TEST_F(FromGWSPageLoadMetricsLoggerTest, IsGoogleSearchRedirectorUrl) {
  struct {
    bool expected_result;
    const char* url;
  } test_cases[] = {
      {true, "https://www.google.com/url?source=web"},
      {true, "https://www.google.com/url?source=web#foo"},
      {true, "https://www.google.com/searchurl/r.html#foo"},
      {true, "https://www.google.com/url?a=b&source=web&c=d"},
      {false, "https://www.google.com/?"},
      {false, "https://www.google.com/?url"},
      {false, "https://www.example.com/url?source=web"},
      {false, "https://google.com/url?"},
      {false, "https://www.google.com/?source=web"},
      {false, "https://www.google.com/source=web"},
      {false, "https://www.example.com/url?source=web"},
      {false, "https://www.google.com/url?"},
      {false, "https://www.google.com/url?a=b"},
  };
  for (const auto& test : test_cases) {
    EXPECT_EQ(test.expected_result,
              FromGWSPageLoadMetricsLogger::IsGoogleSearchRedirectorUrl(
                  GURL(test.url)))
        << "for URL: " << test.url;
  }
}

TEST_F(FromGWSPageLoadMetricsLoggerTest, QueryContainsComponent) {
  struct {
    bool expected_result;
    const char* query;
    const char* component;
  } test_cases[] = {
      {true, "a=b", "a=b"},
      {true, "a=b&c=d", "a=b"},
      {true, "a=b&c=d", "c=d"},
      {true, "a=b&c=d&e=f", "c=d"},
      {true, "za=b&a=b", "a=b"},
      {true, "a=bz&a=b", "a=b"},
      {true, "a=ba=b&a=b", "a=b"},
      {true, "a=a=a&a=a", "a=a"},
      {true, "source=web", "source=web"},
      {true, "a=b&source=web", "source=web"},
      {true, "a=b&source=web&c=d", "source=web"},
      {false, "a=a=a", "a=a"},
      {false, "", ""},
      {false, "a=b", ""},
      {false, "", "a=b"},
      {false, "za=b", "a=b"},
      {false, "za=bz", "a=b"},
      {false, "a=bz", "a=b"},
      {false, "za=b&c=d", "a=b"},
      {false, "a=b&c=dz", "c=d"},
      {false, "a=b&zc=d&e=f", "c=d"},
      {false, "a=b&c=dz&e=f", "c=d"},
      {false, "a=b&zc=dz&e=f", "c=d"},
      {false, "a=b&foosource=web&c=d", "source=web"},
      {false, "a=b&source=webbar&c=d", "source=web"},
      {false, "a=b&foosource=webbar&c=d", "source=web"},
  };
  for (const auto& test : test_cases) {
    EXPECT_EQ(test.expected_result,
              FromGWSPageLoadMetricsLogger::QueryContainsComponent(
                  test.query, test.component))
        << "For query: " << test.query << " with component: " << test.component;
  }
}

TEST_F(FromGWSPageLoadMetricsLoggerTest, QueryContainsComponentPrefix) {
  struct {
    bool expected_result;
    const char* query;
    const char* component;
  } test_cases[] = {
      {true, "a=b", "a="},
      {true, "a=b&c=d", "a="},
      {true, "a=b&c=d", "c="},
      {true, "a=b&c=d&e=f", "c="},
      {true, "za=b&a=b", "a="},
      {true, "ba=a=b&a=b", "a="},
      {true, "q=test", "q="},
      {true, "a=b&q=test", "q="},
      {true, "q=test&c=d", "q="},
      {true, "a=b&q=test&c=d", "q="},
      {false, "", ""},
      {false, "za=b", "a="},
      {false, "za=b&c=d", "a="},
      {false, "a=b&zc=d", "c="},
      {false, "a=b&zc=d&e=f", "c="},
      {false, "a=b&zq=test&c=d", "q="},
      {false, "ba=a=b", "a="},
  };
  for (const auto& test : test_cases) {
    EXPECT_EQ(test.expected_result,
              FromGWSPageLoadMetricsLogger::QueryContainsComponentPrefix(
                  test.query, test.component))
        << "For query: " << test.query << " with component: " << test.component;
  }
}

TEST_F(FromGWSPageLoadMetricsLoggerTest, Basic) {
  FromGWSPageLoadMetricsLogger logger;
  ASSERT_FALSE(logger.ShouldLogMetrics(GURL(kExampleUrl)));
}

TEST_F(FromGWSPageLoadMetricsLoggerTest, NoPreviousPage) {
  FromGWSPageLoadMetricsLogger logger;
  logger.SetPreviouslyCommittedUrl(GURL());
  logger.set_navigation_initiated_via_link(true);
  ASSERT_FALSE(logger.ShouldLogMetrics(GURL(kExampleUrl)));
}

TEST_F(FromGWSPageLoadMetricsLoggerTest, NavigationNotInitiatedViaLink) {
  FromGWSPageLoadMetricsLogger logger;
  logger.SetPreviouslyCommittedUrl(GURL(kGoogleSearchResultsUrl));
  logger.set_navigation_initiated_via_link(false);
  ASSERT_FALSE(logger.ShouldLogMetrics(GURL(kExampleUrl)));
}

TEST_F(FromGWSPageLoadMetricsLoggerTest, ProvisionalNonHttpOrHttpsScheme) {
  FromGWSPageLoadMetricsLogger logger;
  logger.SetPreviouslyCommittedUrl(GURL(kGoogleSearchResultsUrl));
  logger.SetProvisionalUrl(GURL("intent://foo"));
  ASSERT_FALSE(logger.ShouldLogMetrics(GURL::EmptyGURL()));
}

TEST_F(FromGWSPageLoadMetricsLoggerTest, ProvisionalFromGWS) {
  FromGWSPageLoadMetricsLogger logger;
  logger.SetPreviouslyCommittedUrl(GURL(kGoogleSearchResultsUrl));
  logger.SetProvisionalUrl(GURL(kGoogleSearchResultsUrl));
  ASSERT_FALSE(logger.ShouldLogMetrics(GURL::EmptyGURL()));
}

TEST_F(FromGWSPageLoadMetricsLoggerTest, ProvisionalNotFromGWS) {
  FromGWSPageLoadMetricsLogger logger;
  logger.SetPreviouslyCommittedUrl(GURL(kGoogleSearchResultsUrl));
  logger.SetProvisionalUrl(GURL(kExampleUrl));
  ASSERT_TRUE(logger.ShouldLogMetrics(GURL::EmptyGURL()));
}

TEST_F(FromGWSPageLoadMetricsLoggerTest, ProvisionalIntent) {
  FromGWSPageLoadMetricsLogger logger;
  logger.SetPreviouslyCommittedUrl(GURL(kGoogleSearchResultsUrl));
  logger.SetProvisionalUrl(GURL("intent://en.m.wikipedia.org/wiki/Test"));
  ASSERT_FALSE(logger.ShouldLogMetrics(GURL::EmptyGURL()));
}

TEST_F(FromGWSPageLoadMetricsLoggerTest, ProvisionalIgnoredAfterCommit1) {
  FromGWSPageLoadMetricsLogger logger;
  logger.SetPreviouslyCommittedUrl(GURL(kGoogleSearchResultsUrl));
  logger.SetProvisionalUrl(GURL(kExampleUrl));
  logger.set_navigation_initiated_via_link(true);
  ASSERT_FALSE(logger.ShouldLogMetrics(GURL(kGoogleSearchResultsUrl)));
}

TEST_F(FromGWSPageLoadMetricsLoggerTest, ProvisionalIgnoredAfterCommit2) {
  FromGWSPageLoadMetricsLogger logger;
  logger.SetPreviouslyCommittedUrl(GURL(kGoogleSearchResultsUrl));
  logger.SetProvisionalUrl(GURL(kGoogleSearchResultsUrl));
  logger.set_navigation_initiated_via_link(true);
  ASSERT_TRUE(logger.ShouldLogMetrics(GURL(kExampleUrl)));
}

TEST_F(FromGWSPageLoadMetricsLoggerTest, NavigationFromSearch) {
  FromGWSPageLoadMetricsLogger logger;
  logger.SetPreviouslyCommittedUrl(GURL(kGoogleSearchResultsUrl));
  logger.set_navigation_initiated_via_link(true);
  ASSERT_TRUE(logger.ShouldLogMetrics(GURL(kExampleUrl)));
}

TEST_F(FromGWSPageLoadMetricsLoggerTest, NavigationToSearchHostname) {
  FromGWSPageLoadMetricsLogger logger;
  logger.SetPreviouslyCommittedUrl(GURL(kGoogleSearchResultsUrl));
  logger.set_navigation_initiated_via_link(true);
  ASSERT_FALSE(logger.ShouldLogMetrics(GURL("https://www.google.com/about/")));
}

TEST_F(FromGWSPageLoadMetricsLoggerTest, NavigationFromSearchRedirector) {
  FromGWSPageLoadMetricsLogger logger;
  logger.SetPreviouslyCommittedUrl(
      GURL("https://www.google.com/url?source=web"));
  logger.set_navigation_initiated_via_link(true);
  ASSERT_TRUE(logger.ShouldLogMetrics(GURL(kExampleUrl)));
}
