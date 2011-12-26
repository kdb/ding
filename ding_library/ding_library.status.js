/**
 * @file
 * JavaScript behavior to update library open/closed status dynamically.
 */

(function ($) {
  "use strict";

  /**
  * Prototype for library opening status indicators.
  */
  Drupal.DingLibraryStatusIndicator = function (options) {
    var self = this;

    // Constructor for the updater.
    self.constructor = function () {
      self.isOpen = false;
      self.options = options;

      self.date = options.date;
      self.nid = options.nid;

      return self;
    };

    // Recalculate opening status for a library.
    // Returns true if library is open, false if not.
    self.calculateOpenStatus = function () {
      var instances, isOpen = false;

      // Get opening hours instances for the date in question.
      instances = Drupal.OpeningHours.dataStore[self.nid][self.date.getISODate()] || [];

      $.each(instances, function () {
        var open = this.start_time.split(":"),
            close = this.end_time.split(":"),
            hours = self.date.getHours(),
            minutes = self.date.getMinutes();

        // Now we have all the data we need, figure out if we're open.
        if ((hours > open[0] ||
            hours === open[0] && minutes >= open[1]) &&
            (hours < close[0] ||
            hours === close[0] && minutes < close[1])) {
          self.isOpen = true;
        }
      });
    };

    // Render the current opening status.
    self.render = function () {
      if (Drupal.OpeningHours.dataStore[self.nid]) {
        self.calculateOpenStatus();
      }

      // Add our element to the DOM, if neccessary.
      if (!self.el) {
        self.el = $('<div class="library-openstatus"></div>');
        self.el.appendTo($(self.options.container).parent('.node-teaser-library').find('.picture'));

        // Save the view instance for later reference.
        self.el.data('statusIndicatorInstance', self);
      }

      if (self.isOpen) {
        self.el.removeClass('closed');
        self.el.addClass('open');
        self.el.text(Drupal.t('Open'));
      }
      else {
        self.el.addClass('closed');
        self.el.removeClass('open');
        self.el.text(Drupal.t('Closed'));
      }
    };

    return self.constructor();
  };

  // Set up our status indicators when the document is loaded.
  $(window).bind('OpeningHoursLoaded', function () {
    var date = new Date();

    // Set up DingLibraryStatusIndicator instances for each presentation
    // present on the page.
    $('.opening-hours-week').each(function () {
      var indicator = new Drupal.DingLibraryStatusIndicator({
        container: this,
        date: date,
        nid: parseInt($(this).attr('data-nid'), 10)
      });

      indicator.render();
    });
  });

}(jQuery));

