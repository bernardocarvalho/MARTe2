echo "Iniciando make's:"
cd $MARTe2_Components_DIR/Source/Components/Interfaces/NcursesInterface
make -f Makefile.x86-linux
cd $MARTe2_Components_DIR/Source/Components/GAMs/KeyPressedGAMWithSignals
make -f Makefile.x86-linux
cd $MARTe2_Components_DIR/Source/Components/GAMs/KeyActionGAMWithSignals
make -f Makefile.x86-linux
cd $MARTe2_DIR/Docs/User/source/tutorial/Example9
