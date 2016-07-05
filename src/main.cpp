// Copyright 2016 Phil Homan

#include <stdio.h>

#include <vector>

#include "spdlog/spdlog.h"

#include "SolaireConfig.h"
#include "app/app.h"

int main(int argc, char *argv[]) {
  // Create the logger with sinks to stdout
  // and a daily log file that rotates at 00:00.
  std::vector<spdlog::sink_ptr> sinks;
  sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_st>());
  sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_st>(
      "SolaireLog", "txt", 0, 0));
  auto log = std::make_shared<spdlog::logger>("consoleAndFile", begin(sinks),
                                              end(sinks));
  // TODO(phil): Customize the logger format.
  spdlog::register_logger(log);

  log->info("S~O~L~A~I~R~E Version {}.{}", Solaire_VERSION_MAJOR,
            Solaire_VERSION_MINOR);
  log->info("PRAISE THE SUN!");

  log->info("Program Args: ");
  for (int i = 0; i < argc; i++) {
    log->info("Arg{}: {}", i, argv[i]);
  }
  log->info("Done printing program args.");

  solaire::app::gp_app = new solaire::app::App();
  log->info("Created instance of App()");
  if (solaire::app::gp_app == nullptr) {
    log->info("Somehow gp_app is nullptr.");
  }
  if (!solaire::app::gp_app->InitInstance()) {
    log->critical("Failed to initalize app.");
    solaire::app::gp_app->Shutdown();
    delete solaire::app::gp_app;
    return -1;
  }

  while (solaire::app::gp_app->IsRunning()) {
    solaire::app::gp_app->Run();
  }

  solaire::app::gp_app->Shutdown();
  int exit_code = solaire::app::gp_app->GetExitCode();
  delete solaire::app::gp_app;

  return exit_code;
}
