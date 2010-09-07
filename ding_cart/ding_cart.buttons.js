// $Id$

/**
 * Javascript helpers for Ding cart interaction buttons.
 */
Drupal.behaviors.dingCartButtons = function () {
  $("ul.ding-cart-buttons a").click(function (event) {
      // Make sure we grab the click.
      event.preventDefault();

      var $button = $(this);

      if (!$button.hasClass('disabled')) {
        // Make the request back to Drupal.
        $.post(this.href, {}, function (data) {
          var buttons, $count, message;
          // Message is overwritten by the data attribute.
          message = '';
          buttons = {};
          buttons[Drupal.t('Close')] = function () {
            $(this).dialog('close');
          };

          if (data) {
            message = data.message;
            if (data.status === 'success' && $button.hasClass('add-to-cart')) {
              buttons[Drupal.t('View cart…')] = function () {
                window.location = data.cart_link;
              };
              $count = $('#block-ding-user-account .cart .count');
              $count.text(parseInt($count.text(), 10) + 1);
            }
          }
          else {
            message = Drupal.t('An error occurred.');
          }

          $('<div>' + message + '</div>')
            .dialog({
              'title': data.title,
              'buttons': buttons,
              'close': function (event, ui) {
                $(this).dialog('destroy').remove();
              }
            });
        }, 'json');
      }

      return false;
  });
};

