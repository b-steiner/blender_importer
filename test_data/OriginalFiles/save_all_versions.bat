cd ..
for /D %%i in (C:\Users\bsteiner\Downloads\blender\*) do start /wait %%i\blender.exe --background -y -P OriginalFiles\save.py