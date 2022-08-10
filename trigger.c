#include "trigger.h"
#include "context.h"
#include "log.h"
#include "flow.h"
#include "condition.h"

int run_trigger(const apermon_config_triggers *config, const apermon_flows *flows) {
    apermon_context *ctx = config->ctx;
    apermon_hash_item *aggr;
    apermon_aggregated_flow *af;
    uint64_t bps, pps;

    const apermon_flow_record *r = flows->records;
    const apermon_aggregated_flow_average *avg;

    ctx->now = time(NULL);
    ctx->current_flows = flows;
    ctx->n_selected = 0;

    if (ctx->now - ctx->last_gc >= CONTEXT_GC_MIN_INTERVAL) {
        gc_context(ctx);
    }

    cond_begin(ctx);

    while (r != NULL) {
        if (config->conds == NULL || cond_list(r, config->conds)) {
            select_flow(r);
        }

        r = r->next;
    }

    if (aggergrate_flows(ctx) < 0) {
        log_warn("internal error: aggergrate_flows failed.\n");
    }

    aggr = ctx->aggr_hash->head;

    dump_flows(ctx, 1);

    while (aggr != NULL) {
        af = (apermon_aggregated_flow *) aggr->value;

        if (!af->dirty) {
            aggr = aggr->iter_next;
            continue;
        }

        af->dirty = 0;
        avg = running_average(af);
        
        bps = avg->in_bps > avg->out_bps ? avg->in_bps : avg->out_bps;
        pps = avg->in_pps > avg->out_pps ? avg->in_pps : avg->out_pps;

        if (config->bps > 0) {
            if (bps >= config->bps) {
                log_info("trigger: %s (bps: %lu > %lu)\n", config->name, bps, config->bps);
                // todo
            }
        } else if (config->pps > 0) {
            if (pps >= config->pps) {
                log_info("trigger: %s (pps: %lu > %lu)\n", config->name, pps, config->pps);
                // todo
            }
        }

        aggr = aggr->iter_next;
    }

    return 0;
}