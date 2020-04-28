#  Network Programming: Milestone 5

Compile: sh compile.sh (set path in /srv/www/cgi-bin for Apache2 in openSuse 11.2)
Run: On the browser type: localhost//cgi-bin/cscript.cgi

 1.  $_SERVER is an array containing information such as headers, paths, and script locations.
 The entries in this array are created by the web server. Mostly there is no guarentee that
 all entries are implemented by the web server and are accounted in CGI specification.

 2.  $_GET is an associative array of variables passed to the current script via the URL parameters.
 Generally contains in the QUERY_STRING of the server env with name and values seperated by
 amphersand '&'

 3.  $_POST is an associative array of variables passed to the current script via the HTTP POST method.
 Generally taken by standard input by the server with its length determined from the CONTENT_LENGTH
 set in the server environment.

 4.  $_COOKIE is an associative array of variables passed to the current script via HTTP Cookies. Generally
 contains in the HTTP_COOKIE of the server env with id and values seperated by semicolon ';'

 5.  Function: void header  (  string $string,  bool $replace,  int $http_response_code )
 Used to send raw HTTP header. MUST be called before any html output script.

 String: Contains the header string.
 Replace: Determines whether to replace the previous header values. By default it is true.
 Response Code: Forces the response code when the response is send by the web server.

 6.  Function: bool setcookie  (  string $name, string $value, int $expire, string $path, string $domain, bool $secure, bool $httponly)
 Defines a cookie to be sent along with the HTTP headers. Similar to header it MUST be called before any html output script.

 Name: Name of the Cookie.
 Value: Value of the cookie.
 Expire: Expiration time for the cookie in seconds. By default it is set to 0 indicating its expiration after closing of browser.
 Path: Path of server were the cookie will be available.
 Domain: The domain in which the cookie is available.
 Secure: When it is true, the cookie is transmitted using secure connection such as HTTPS. By default it is set to false.
 Httponly: When it is true, the cookie is accessible only using HTTP protocol. By default it is set to false.


## The Implementation consists of 4 parts:

1. Linklist Data Structure (list.h, list.c): It consists of the core data structure mainly to store GET, POST, COOKIE arrays along with
   the inner working HEADER array for manipulation of headers. It has basic methods to add, update, show or dump to display in varying
   formats, free the nodes in the linked list. The main list is denote by List while nodes are denoted by structure Data.

2. Global PHP Array Initialization (globals.h, globals.c): Sets the global arrays for PHP such as SERVER, GET, POST, COOKIE, using the
   getenv() i.e. get environment variables or from standard input in cast of POST method. All the strings are parsed and stored in the
   arrays in form of linked list structure declared before.

3. PHP Library (phplib.h, phplib.c): Contains the implementation of 3 main PHP functions i.e. header(), setcookie() and print_r().
   They modify the values of corresponding global arrays initialized before and attempt to change values in the environment which
   is not updated using the setenv method.

4. Test CGI Script (cscript.c): The Test CGI Script is a simple script which calls the defined PHP functions in phplib library and
   displays the status of the global arrays in its main method.


## Execution:
 * To compile the program execute the shell script compile.sh using command 'sh compile.sh'.
 * Add compiled directory path to /etc/ld.so.conf and then execute command 'ldconfig' in super user mode.
 * Type "http://localhost/gtest.html" to test the GET or "http://localhost/ptest.html" to test the POST method
    (Note: gtest.htmland ptest.html are present in htdocs folder while the cgi script and library files along
     with compile shell script are present in cgi-bin folder).


## Issues:

 1) Apache2 webserver configuration by configuring /srv/www/cgi-bin directory using <Directory> tags and setting up an
    ScriptAlias for '/cgi-bin/'. Permissions for /srv/www/htdocs and /srv/www/cgi-bin should be set for access of scripts to
    Webserver.

 2) Problem with the compilation of shared libraries and allowing access of global variables to all the files in the shared
    library. Requires to append the current dir path containing the "libphplib.so" file as library path in /etc/ld.so.conf
    in super user mode. Also run command ldconfig as root to load the libraries from the /etc/ld.so.conf in the system.
    Also need to export LD_LIBRARY_PATH set to current directory path containing "libphplib.so" file as done in compile.sh

 3) The SetEnv() does not change or update the environment variables, thus not allowing the setcookie method to permanently
    set a cookie, making the setting of the cookie temperorary. Similarly for other parameters as well as headers.
