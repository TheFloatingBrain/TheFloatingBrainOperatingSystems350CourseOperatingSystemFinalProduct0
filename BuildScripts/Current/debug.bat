wsl bash -c "./build.sh -clean"
cd ../../simulator-master/
java -jar simulator.jar ../IMG/Current/floppya.img
cd ../BuildScripts/Current
