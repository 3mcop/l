
(function ($) {
  // Make sure Wysiwyg ImageField objects are defined.
  Drupal.wysiwygImageField = Drupal.wysiwygImageField || {};
  Drupal.wysiwygImageField.hookSetPosition = Drupal.wysiwygImageField.hookSetPosition || {};

  Drupal.wysiwyg.plugins['wysiwyg_imagefield'] = {
    /**
     * Return whether the passed node belongs to this plugin.
     */
    isNode: function(node) {
      return false;
    },

    /**
     * Execute the button.
     */
    invoke: function(data, settings, instanceId) {
      $('#wysiwyg_imagefield-wrapper').dialog('open');
      Drupal.wysiwygImageField.focus();
      Drupal.wysiwygImageField.setPosition();
    },

    /**
     * Create wysiwyg_imagefield dialog window.
     */
    attach: function(content, settings, instanceId) {
      // @TODO: Use Modal Frame API if available.
      $('#wysiwyg_imagefield-wrapper').dialog({
        autoOpen: false,
        modal: true,
        title: Drupal.settings.WysiwygImageField.title
      });
      Drupal.wysiwygImageField.init();
      Drupal.wysiwygImageField.focus();
      return content;
    },

    /**
     * Replace images with <!--break--> tags in content upon detaching editor.
     */
    detach: function(content, settings, instanceId) {
      return content;
    }
  };

  Drupal.wysiwygImageField = {
    init: function() {
      $('#wysiwyg_imagefield-wrapper').parents('.ui-dialog').attr('id', 'wysiwyg_imagefield-dialog');

      // Move dialog back inside form.
      $('#wysiwyg_imagefield-dialog').bind('focus', function() {
        Drupal.wysiwygImageField.focus();
      });
    },

    focus: function() {
      if ($('#wysiwyg_imagefield-dialog').parent() !== $('#' + Drupal.wysiwyg.activeId).parent()) {
        $('#wysiwyg_imagefield-dialog').prependTo($('#' + Drupal.wysiwyg.activeId).parent());
      }
    },

    getId: function() {
      // Use tallest field as target.
      height = 0;
      id = null;
      $('#' + Drupal.wysiwyg.activeId).parent().children().each(function() {
        if ($(this).css('display') !== 'none' && $(this).height() > height && $(this).attr('id') != 'wysiwyg_imagefield-dialog')  {
          height = $(this).height();
          if (!$(this).attr('id')) {
            $(this).attr('id', 'wysiwyg_imagefield-' + (new Date().getTime()) + Math.ceil(Math.random() * 100));
          }
          id = '#' + $(this).attr('id');
        }
      });
      return id;
    },

    setPosition: function() {
      id = Drupal.wysiwygImageField.getId();

      // Position dialog.
      $('#wysiwyg_imagefield-dialog').css({
      ////  height: null
        left: $(id).position().left + 35,
      ////  //overflow: 'visible'
      ////  //top: $(id).position().top + 50,
        width: $(id).width() - 100 > 475 ? $(id).width() - 100 : 475
      });
      //$('#wysiwyg_imagefield-wrapper').css({
      //  height: null,
      //  width: null
      //});

      // Invoke Drupal.wysiwygImageField.hookSetPosition().
      if (Drupal.wysiwygImageField.hookSetPosition != undefined) {
        $.each(Drupal.wysiwygImageField.hookSetPosition, function() {
          if ($.isFunction(this)) {
            this();
          }
        });
      }
    }
  }

  Drupal.behaviors.wysiwygImageField = {
    attach: function(context) {
      $('#wysiwyg_imagefield-wrapper .insert-button').click(function() {
        $('#wysiwyg_imagefield-wrapper').dialog('close');
        $('#wysiwyg_imagefield-wrapper .content-add-more input:submit').mousedown();
        $($('#wysiwyg_imagefield-wrapper').children().get(1)).remove();
      });
    }
  }
})(jQuery);
