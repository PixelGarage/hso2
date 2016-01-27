-----------------------------------------------------------------------------
ABOUT THIS MODULE
-----------------------------------------------------------------------------
The Isotope module provides several Views Style plugins. The isotope_views style plugin allows to layout
an unformatted list of items with the famous layouts styles of the Isotope Javascript library of Metafizzy.

- The isotope_filter_views style plugin creates filter button groups to filter items in a Isotope layout view.
- The isotope_sort_views style plugin creates sort button groups to sort items in a Isotope layout view.
- The isotope_iscroll_views pager plugin creates a infinite scrolling pager for an Isotope layout view.

This module depends on the famous Metafizzy-Isotope js-library, which can be found at the following locations:

Code:           See https://github.com/metafizzy/isotope
Documentation:  See http://isotope.metafizzy.co


-----------------------------------------------------------------------------
License
-----------------------------------------------------------------------------
The metafizzy-isotope for commercial use has to be licensed.

See http://isotope.metafizzy.co/license.html


-----------------------------------------------------------------------------
INSTALLATION
-----------------------------------------------------------------------------
Install Isotope Library:
This module requires the Isotope packages installed as drupal library in the sites/all/libraries folder.
Follow these steps to create the Isotope library:

1. Install and enable Libraries API module (see https://www.drupal.org/project/libraries).
2. Create an folder called 'isotope' in the libraries directory (sites/all/libraries).
3. Download the latest release of the Isotope package from https://github.com/metafizzy/isotope/releases.
4. There are many files in this package. You only need to copy isotope.pkgd.min.js (and isotope.pkgd.js, if debug is needed)
    to the isotope folder.
5. Create a folder called 'layout-modes' in the 'isotope' folder.
6. Copy all needed layout modes of the Isotope control to this folder. Available layouts are (see http://isotope.metafizzy.co/layout-modes.html):
    - masonry (included in Isotope package in folder js/layout-modes)
    - fir-rows (included in Isotope package in folder js/layout-modes)
    - vertical (included in Isotope package in folder js/layout-modes)
    - packagery (separate download http://packery.metafizzy.co)
    - cellsByRow (separate download http://isotope.metafizzy.co/layout-modes/cellsbyrow.html)
    - masonryHorizontal (separate download http://isotope.metafizzy.co/layout-modes/masonryhorizontal.html)
    - fitColumns (separate download http://isotope.metafizzy.co/layout-modes/fitcolumns.html)
    - cellsByColumn (separate download http://isotope.metafizzy.co/layout-modes/cellsbycolumn.html)
    - horizontal (separate download http://isotope.metafizzy.co/layout-modes/horizontal.html)

The final structure looks like this:

sites/all/libraries
  |_ isotope
        |_ layout-modes
        |       |_ masonry.js
        |       |_ vertical.js
        |       |_ fit-rows.js
        |       |_ ...
        |
        |_ ImagesLoaded.pkgd.min.js
        |_ ImagesLoaded.pkgd.js
        |_ Isotope.pkgd.min.js
        |_ Isotope.pkgd.js


Install Isotope module:
See official Drupal documentation for module installation instructions.
--> You then have a new views style plugin available called 'Isotope', that allows you to layout item lists with
    isotope layouts. The plugin can be configured with several settings to adapt the layout modes as you wish.

    Sub-modules allow to define filter button groups (isotope_filter_views) and sort button groups (isotope_sort_views)
    and to add infinite scrolling (isotope_iscroll_views).


