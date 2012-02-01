Custom Breadcrumbs 7.x-1.x
--------------------------

Summary
-------
* Enable the module
* Click Permissions to assign user permissions for this module
* Assign 'administer custom breadcrumbs' permission to those roles that should
  be allowed to add/edit/delete custom breadcrumbs.
* Assign 'use php in custom breadcrumbs' to roles that should be allowed to use
  php to determine breadcrumb visibility.
* Go to Administer > Structure > Custom breadcrumbs to add new breadcrumbs
* Click "Add a new custom breadcrumb"
* Choose the node type to create a breadcrumb trail for
* For the titles, put each "crumb" one line after another (There is no need to
  put in "home"):

  Item 1
  SubItem A
  SuperSubItem X

* For the paths, put the path to each crumb starting after the domain name.
  Don't include a leading or trailing slash.

  item1
  item-1/subitem-a
  item-1/subitem-a/supersubitem-x

* Click save to save the breadcrumb
* Visit the page and your breadcrumb should appear!

Description
-----------
As the name suggests, Custom Breadcrumbs allows you to create and modify your
own breadcrumbs based on node type. After enabling the module, go to
Administer > Structure  > Custom breadcrumbs. You'll be abel to add new
breadcrumbs from this page.

From that page you have the option to select the node type the
breadcrumb will apply to. There are two text fields below-- "Titles" and 
"Paths." When creating a breadcrumb, you're simply creating a link. In the
custom breadcrumbs interface "Titles" describes the text of the breadcrumb
while "Paths" describes the Drupal path the breadcrumb links to. Each Title
must have a corresponding Path.

To give a very simple example of how to use this module, let's say I have a
blog on my web site called "Deep Thoughts." To create this, I use the Views
module to create a page at /blog that displays all the node types "blog post."
Whenever a user views a blog post I want the breadcrumb to show
Home > Deep Thoughts instead of simply Home. To do this I would simply type
"Deep Thoughts" in the "Titles" field and and "blog" in the "Paths" field and
save my breadcrumb.

Using the Tokens module, the Custom breadcrumbs module becomes much more
flexible because breadcrumbs can become dynamic. You can create a breadcrumb
like Home > Deep Thoughts > [Month of Blog Post] [Year of Blog Post], where
"Deep Thoughts" links to my main blog page and "[Month of Blog Post]
[Year of Blog Post]" links to a view that shows only blog posts from the month
and year the blog post was created (e.g. June 2007). For this, you would do
the following:

Node Type:
Blog Post

Titles:
Deep Thoughts
[node:created:custom:M Y]

Paths:
blog
blog/[node:created:custom:m_Y]

(where of course, blog/[node:created:custom:m_Y] is the path to the view of
blog posts from that month and year). So if you created a blog post on
June 13, 2007 your breadcrumb would show Home > Deep Thoughts > June 2007 and
"June 2007" links to "blog/06_2007" which is a view of all blog posts from
June 2007.

Also, note that Custom Breadcrumbs doesn't actually check to be sure that a
particular path exists, so you'll have to check yourself to avoid 404 errors.

Only users with 'administer custom breadcrumbs' permission will be allowed to
create or modify custom breadcrumbs.

Breadcrumb Visibility
---------------------
Users given 'use php in custom breadcrumbs' permission can include a php code
snippet that returns TRUE or FALSE to control whether or not the breadcrumb is
displayed. Note that this code has access to the $node variable, and can check
its type or any other property. Tokens should not be used in the visibility
code snippet, since they will not be replaced.

Special Identifiers
-------------------
The following identifiers can be used to achieve a special behavior:
<pathauto> - will clean any path using the current pathauto module settings,
             if that module is installed.
<none>     - can be used as a path to have a breadcrumb element that is not
             hyperlinked.

Identifiers should be added to the paths area in the following format:
identifier|path. To be recognized, the identifier must be enclosed in angular
brackets, and proceed any part of the path:

For example: <pathauto>|[node:title]
