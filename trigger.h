#ifndef APERMON_TRIGGER_H
#define APERMON_TRIGGER_H
#include "config.h"
#include "extract.h"

int run_trigger(const apermon_config_triggers *config, const apermon_flows *flows);

#endif // APERMON_TRIGGER_H