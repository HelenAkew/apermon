#ifndef APERMON_EXTRACT_H
#define APERMON_EXTRACT_H
#include <stdint.h>
#include "sflow.h"

/* structs for extracted flow */

typedef struct _apermon_flow_records {
    uint32_t flow_af; /* enum sflow_af */
    
    uint32_t seq;
    uint32_t rate;
    uint32_t pool;
    uint32_t in_ifindex;
    uint32_t out_ifindex;
    uint32_t frame_length;

    union {
        uint32_t src_inet;
        uint8_t src_inet6[16];
    };

    union {
        uint32_t dst_inet;
        uint8_t dst_inet6[16];
    };

    uint8_t l3_proto;
    uint16_t src_port; // valid iff l3proto = tcp or udp
    uint16_t dst_port; // valid iff l3proto = tcp or udp

    struct _apermon_flow_records *next;
} apermon_flow_record;

typedef struct _apermon_flows {
    uint32_t agent_af; /* enum sflow_af */
    union {
        uint32_t agent_inet;
        uint8_t agent_inet6[16];
    };

    uint32_t sub_agent_id;
    uint32_t seq;
    uint32_t uptime;

    apermon_flow_record *records;
} apermon_flows;


/* functions */
int extract_flows(const sflow_parsed *parsed, apermon_flows **flows);
void free_apermon_flows(apermon_flows *flows);

#endif // APERMON_EXTRACT_H