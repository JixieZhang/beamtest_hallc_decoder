{
    //gSystem->SetBuildDir(".root_build_dir", true);
    // analyzer
    //gInterpreter->AddIncludePath("./decoder");
    //gSystem->Load("./build/decoder/libfdec.so");
    gInterpreter->AddIncludePath("./include");
    gSystem->Load("./lib64/libfdec.so");


    // online monitor
    /*
    gInterpreter->AddIncludePath("./online");
    gInterpreter->AddIncludePath("./third_party/et/include/coda/et");
    gSystem->Load("./third_party/et/lib64/libcoda_et.so");
    */
}
