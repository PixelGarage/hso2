The Flexpaper2 module provides a formatter for the File field that can display pdf file(s)
using the Devaldi FlexPaper flipbook viewer.

See also http://flexpaper.devaldi.com

-----------------------------------------------------------------------------
VERSION INFO
-----------------------------------------------------------------------------
Version 1.x supports the upload of PDF files and archives (.zip, .tar.gz, etc.) of a locally published PDF file
with the Flexpaper Desktop Publisher Zine application. See below for more informations.

The actual version of Flexpaper Desktop Publisher Zine at development time was Version 2.3.8.

-----------------------------------------------------------------------------
ABOUT THIS MODULE
-----------------------------------------------------------------------------
This module provides a field formatter for the File field. This formatter allows
to display PDF files as beautiful adaptive flipbooks, using the Devaldi Flexpaper
Viewer. You can see demos of published PDF's at http://flexpaper.devaldi.com/demo.jsp

The module supports two publishing methods, the "Client Publishing" method and the "Server Publishing" method:

1) Client publishing: Publish a PDF locally with the "FlexPaper Desktop Publisher Zine" application (Mac and Windows).
   Compress the resulting directory and upload the archive instead of the pdf (all Drupal archive formats are supported).
   This possibility has no server requirements and therefore can be used on all web servers.

2) Server publishing: Upload a pdf and process it on the server, so that the Flexpaper Viewer can
   display it. This variant needs specific conversion tools installed on the server,
   e.g. pdf2swf, pdf2json, swfrender and pdftk, and therefore needs most likely administrator access on the server.

The commercial version of the Flexpaper viewer supports 3 publishing formats: html5, html4 and Flash.
All three formats are supported, but the Adaptive UI can only be used in html5/html4 modes.

Remark: On html5 websites the Flexpaper viewer can display a PDF file without processing it on the server!
But bigger PDFs are not as performant as published PDFs are (support PDF split into pages and progressive loading).


-----------------------------------------------------------------------------
INSTALLATION
-----------------------------------------------------------------------------
Flexpaper Library:
This module requires the FlexPaper Webserver package available as drupal library. Follow these steps to create the
flexpaper library:

1. Install and enable Libraries API module (see https://www.drupal.org/project/libraries).
2. Create a flexpaper folder in the libraries directory (sites/all/libraries).
3. Download the needed Flexpaper Webserver package from the download page http://flexpaper.devaldi.com/download.
4. There are many folders in this package. You need only js, css and locale folders and FlexPaperViewer.swf file.
   Copy these items into the flexpaper folder and that's it.
5. If you want the configuration file for the viewer included rename the UIConfig_{file_name}.pdf.xml to UIConfig_standard.pdf.xml
   and copy it to the flexpaper library folder. Additionally copy the assets_zine folder to the flexpaper library folder.


Install Flexpaper2 module:
See official Drupal documentation for module installation instructions.


--> You now have a new file field formatter available called 'Flexpaper viewer', that allows you to display PDFs
    as Flexpaper Flipbooks.


-----------------------------------------------------------------------------
CLIENT PUBLISHING
-----------------------------------------------------------------------------
The Flexpaper Desktop Publisher Zine application publishes a PDF into the following file structure:

project
    |_ assets_zine
    |_ css
    |_ docs
    |_ js
    |_ locale
    |_ FlexPaperViewer.swf
    |_ ... more files

Just compress the project-folder with a popular compressor like zip or tar and upload this file to the server
to be displayed in the Flexpaper viewer.


-----------------------------------------------------------------------------
SERVER PUBLISHING REQUIREMENTS
-----------------------------------------------------------------------------
Server publishing requires these tools to be installed on the server, which usually means that you need
administrator access on the server. If you do not have these rights use the client publishing method instead:

1. pdf2swf and swfrender: these tools support server publishing for the Flash and HTML4 modes.
2. pdf2json: To provide a correct way to search the text in the split mode your server
   should also have pdf2json tool installed and available on the command line.
3. pdftk (optional): To provide highly performing FlipBooks in HTML5, the pdftk command line tool has to be
   installed on the server (splits PDFs into pdf pages).
4. Your server should allow PHP exec() and shell_exec() to run the specific command line tools.



-----------------------------------------------------------------------------
CREDITS
-----------------------------------------------------------------------------
This project is heavily based on the Flexpaper module as a very good starting point. Many thanks to the great work of
Ivan Strygin (feolius). But due to major changes and enhancements we decided to create a new module instead of enhancing
the existing flexpaper module.

Sponsored by Pixelgarage (http://www.pixelgarage.ch)
