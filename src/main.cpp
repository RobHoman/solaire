// Copyright 2016 Phil Homan

#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "./app.h"
#include "SolaireConfig.h"
#include "./easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[]) {
  LOG(INFO) << "S~O~L~A~I~R~E Version "
            << Solaire_VERSION_MAJOR << "." << Solaire_VERSION_MINOR;
  LOG(INFO) << "PRAISE THE SUN!";
  LOG(INFO) << "Program Args: ";
  for (int i = 0; i < argc; i++) {
    LOG(INFO) << argv[i];
  }

  gp_app = new App();
  if (!gp_app->InitInstance()) {
    LOG(FATAL) << "Failed to initalize app.";
    return -1;
  }

  while (gp_app->IsRunning()) {
    gp_app->Run();
  }

  return gp_app->GetExitCode();
}
