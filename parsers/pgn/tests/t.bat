del *.h~
del *.cpp~
del ..\*.h~
del ..\*.cpp~
bcc32 -etest -I.. ..\*.cpp -I. *.cpp
test
