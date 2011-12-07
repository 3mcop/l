
The Wysiwyg ImageField module is an inline image management tool for the Wysiwyg
module based on the core Image field and Insert module.

Wysiwyg ImageField was written and is maintained by Stuart Clark (deciphered).
- http://stuar.tc/lark


Features
--------------------------

* Support for Wysiwyg libraries:
  * CKEditor/FCKEditor.
  * TinyMCE.
* Support for FileField/ImageField based modules:
  * FileField Paths module.
* Support for CCK Formatters:
  * Core Image styles.
  * Lightbox2 module.
  * etc.


Required modules
--------------------------

* Wysiwyg module.
* Insert module.


Configuration
--------------------------

Wysiwyg ImageField must be enabled and configured on each desired Content type,
follow these simple steps to do so:

1. Create a new Image field or edit an existing Image field on the desired
   Content types 'Manage fields' page.

2. Check the 'Use with Wysiwyg ImageField?' checkbox.
   Note: Only one Image field per Content type can be used.

3. Under the 'Insert' section, check the 'Enable insert button' checkbox and
   configure the 'Enabled insert styles' and 'Default insert style' sections.

4. Set 'Number of values' to 'Unlimited'.

Once Wysiwyg ImageField is configured in your desired Content types, enable the
Wysiwyg plugin on your desired Wysiwyg profile and you are ready to go.


Frequently asked questions
--------------------------

Q. Where did my Image field go?

A. To make the process of adding inline images as streamlined as possible,
   Wysiwyg ImageField removes the Image widget from the user interface,
   making it visible only when needed.

   It is recommended that if you require an Image field accessible outside of
   the Wysiwyg ImageField experience that you create an Image field for the sole
   purpose of using it with Wysiwyg ImageField instead of using your existing
   Image field.


Q. How can I see all the files uploaded to the Wysiwyg ImageField widget?

A. Currently this is not available, for the sole purpose of keeping the system
   as simple as possible.

   Ideas have been put forward and there will be a solution for this in a
   future release.


Known issues
--------------------------

- Dialog doesn't close on Insert in IE6.
- Insert module D7 port issues?
