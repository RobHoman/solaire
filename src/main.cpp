// Copyright 2016 Phil Homan

#include <stdio.h>

#include <vector>

#include "spdlog/spdlog.h"

#include "SolaireConfig.h"
#include "./app.h"

using ::solaire::app::gp_app;

int main(int argc, char *argv[]) {
  // Create the logger with sinks to stdout
  // and a daily log file that rotates at 00:00.
  std::vector<spdlog::sink_ptr> sinks;
  sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_st>());
  sinks.push_back(
      std::make_shared<spdlog::sinks::daily_file_sink_st>(
          "SolaireLog", "txt", 0, 0));
  auto log = std::make_shared<spdlog::logger>(
      "consoleAndFile", begin(sinks), end(sinks));
  // TODO(phil): Customize the logger format.
  spdlog::register_logger(log);

  log->info("S~O~L~A~I~R~E Version {}.{}",
                   Solaire_VERSION_MAJOR, Solaire_VERSION_MINOR);
  log->info("PRAISE THE SUN!");
  log->info("Program Args: {}", argv);

  gp_app = new solaire::app::App();
  if (!gp_app->InitInstance()) {
    log->critical("Failed to initalize app.");
    gp_app->Shutdown();
    delete gp_app;
    return -1;
  }

  while (gp_app->IsRunning()) {
    gp_app->Run();
  }

  gp_app->Shutdown();
  delete gp_app;
  return gp_app->GetExitCode();
}
