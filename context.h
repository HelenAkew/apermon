#ifndef APERMON_CONTEXT_H
#define APERMON_CONTEXT_H
#include "extract.h"
#include "hash.h"
#include "flow.h"
#include "config.h"

typedef struct _apermon_cond_selected_flows apermon_cond_selected_flows;

typedef struct _apermon_context {
    // info about current batch of flow records
    const apermon_flows *current_flows; /* not own by us */
    apermon_cond_selected_flows *selected_flows; /* not own by us */
    apermon_cond_selected_flows *selected_flows_tail; /* not own by us */

    // context (persistent) info
    apermon_hash *aggr_hash; /* hashmap: inet/inet6 to aggr, own by us */
    apermon_config_triggers *trigger_config; /* not own by us */
} apermon_context;

apermon_context *new_context();
void free_context(apermon_context *ctx);

#endif // APERMON_CONTEXT_H