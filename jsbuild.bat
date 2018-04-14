c:\qt\projects\photonboard2\xsc\xsc modules/base/timer/timer.js -d -c -e -o %2
c:\qt\projects\photonboard2\xsc\xsc modules/base/console/console.js -d -c -e -o %2
c:\qt\projects\photonboard2\xsc\xsc modules/dom/dom.js -d -c -e -o %2
c:\qt\projects\photonboard2\xsc\xsl -r prebuild_js %2\console.xsb %2\timer.xsb %2\dom.xsb -o %1 -b %2
