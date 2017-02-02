
Content Theme
=============

Allows to use different themes than the site default on content creating,
editing, and viewing pages.

Installation
------------

- Install this module using the official Backdrop CMS instructions at
  (https://backdropcms.org/guide/modules).

- Enable this module under Administration > Functionality > Appearance.

Usage
-----

- **Content wide themes**  
  Content wide themes applies to all content and override the system default
  theme. But these settings can be overridden by content type and content node
  settings. Configure content wide themes under Administration > Appearance >
  Content theme > Content wide.

- **Content type themes**  
  Content type themes applies to all content based on its content type and
  override content wide themes and the system default theme. But these settings
  can be overridden by content node settings. Configure content type themes at
  its content type edit form or under Administration > Appearance > Content
  theme > Content type.

- **Content node themes**  
  Content node themes applies only to its own content and override the content
  type themes, content wide themes, and the system default theme. Configure
  content node themes at its content node edit form or under Administration >
  Appearance > Content theme > Content node.

Troubleshooting
---------------

If another theme is displayed as expected, enable the Custom Theme Analyzer
block from the Custom Theme Analyzer module. This block displays a list of
modules which override the system default theme sorted by module's call-up.
The Custom Theme Analyzer module is part of the Analyzers collection
(https://backdropcms.org/project/analyzers).

License
-------

This project is GPL v2 software. See the LICENSE.txt file in this directory for
complete text.

Current Maintainers
-------------------

- Ralf Stamm (https://github.com/rstamm).

Credits
-------

- Ported to Backdrop CMS by Ralf Stamm (https://github.com/rstamm).

- Originally written for Drupal by Ralf Stamm (https://github.com/rstamm).
