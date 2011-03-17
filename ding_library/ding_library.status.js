/**
 * @file
 * JavaScript behavior to update library open/closed status dynamically.
 */


/**
 * Prototype for updateting library opening status.
 */
Drupal.DingLibraryStatusUpdater = function () {
  var self = this;

  /**
   * Constructor for the updater.
   */
  self.init = function () {
    if (self.isInitialised) { return; }
    self.isInitialised = true;

    // Get the settings from the Drupal settings object.
    self.settings = Drupal.settings.officeHours['node' + Drupal.settings.dingLibraryNids[0]];

    // When done initialising, call refresh the first time.
    self.reloadData();
  };

  /**
   * Reload the status data from the server.
   */
  self.reloadData = function () {
    $.getJSON(self.settings.callback + '/' + Drupal.settings.dingLibraryNids.join(',') + '/' + self.settings.field_name, {}, function (response, textStatus) {
      $.each(response.data, function (nid, hoursData) {
        $('#node-' + nid + ' .library-openstatus')
          // Update the label.
          .text(hoursData.status_local)
          // Remove the existing status classes.
          .removeClass('open')
          .removeClass('closed')
          // Add the current status as a class.
          .addClass(hoursData.status);
      });
    });
  };

  /**
   * Helper function to reload status regularly.
   */
  self.reloadDataEvery = function (interval) {
    window.clearInterval(self.refreshInterval);

    self.reloadInterval = window.setInterval(self.reloadData, interval);
  };

  self.init();
  return self;
};

// Set up our status update when the document is loaded.
jQuery(function($) {
  // Set up status updater.
  var updater = new Drupal.DingLibraryStatusUpdater();

  // Reload library status data every 10 minutes.
  updater.reloadDataEvery(600000);
});

