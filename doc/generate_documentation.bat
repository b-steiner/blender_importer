echo creating documentation..

cd ..

if exist "doc/blender_importer" rd "doc/blender_importer/" /s /q
md "doc/blender_importer/"

"external/doxygen/doxygen.exe" "doc/doxyfile" 2> "doc/doxygen_error.log"