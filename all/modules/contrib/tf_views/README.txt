
CONTENTS OF THIS FILE
---------------------

 * Author
 * Description
 * Installation
 * Configuration

AUTHOR
------
Jim Berry ("solotandem", http://drupal.org/user/240748)

DESCRIPTION
-----------
This module modifies the default "taxonomy_term" view provided by the Views
module to restore the multi-term node display functionality removed in Drupal 7.
The default 'taxonomy_term' view returns all nodes tagged with ANY of the term
ids in the URL.

In Drupal 6, the 'taxonomy/term/%' path returned a listing of nodes tagged with
ALL of the term ids in the URL. In Drupal 7, this functionality was removed and
attempts to view a multi-term display result in a display entitled "Page not
found" with a message of "The requested page could not be found."

The (Taxonomy Filter) Multi-Term Views module restores this functionality by
modifying the query executed by Views for this display. In addition, the module
adds a database index that dramatically reduces the query execution time for
this display.

This module can be used stand-alone (with Views of course) and in conjunction
with the Taxonomy Filter project. The latter project includes a "Multi-Term
Filter" module (tf_multi) which provides taxonomy filter menus when multiple
term ids are present in the URL.

INSTALLATION
------------
Install the code in a modules directory. See http://drupal.org/node/895232 for
further information.

CONFIGURATION
-------------
After enabling this module:
- enable the default "taxonomy_term" view provided by the Views module
- edit the view
- expand the "Advanced" settings
- click on "Content: Has taxonomy term ID (with depth)" in "Contextual filters"
- scroll down to "Filter value type"
- select "Term IDs separated by , or +"
- click "Apply" to close the overlay
- click "Save" to save the view
- naviagate to "taxonomy/term/A,B" to see if the content displays

With this configuration, displays should be possible of nodes that match ALL or
ANY of the term ids in the URL.
