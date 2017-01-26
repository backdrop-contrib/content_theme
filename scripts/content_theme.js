
(function ($) {

Backdrop.behaviors.contentThemeFieldsetSummaries = {

  attach: function (context) {

    $('fieldset.content-theme-settings-form', context).backdropSetSummary(function (context) {
      var themeEdit = $('.form-item-content-theme-edit select option:selected', context).text();
      var themeView = $('.form-item-content-theme-view select option:selected', context).text();

      return Backdrop.t('Editing: @edit; Viewing: @view', { '@edit': themeEdit, '@view': themeView });
    });
  }
};

})(jQuery);
