@REM rmdir /s/q .\build
cmake -S . -B .\build
cmake --build .\build --target DentalCAD -j 10 --config Release
move build\DentalCAD\Release\DentalCAD.exe .\