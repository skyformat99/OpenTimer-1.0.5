/******************************************************************************
 *                                                                            *
 * Copyright (c) 2016, Tsung-Wei Huang and Martin D. F. Wong,                 *
 * University of Illinois at Urbana-Champaign (UIUC), IL, USA.                *
 *                                                                            *
 * All Rights Reserved.                                                       *
 *                                                                            *
 * This program is free software. You can redistribute and/or modify          *
 * it in accordance with the terms of the accompanying license agreement.     *
 * See LICENSE in the top-level directory for details.                        *
 *                                                                            *
 ******************************************************************************/

#include "ot_dist_agent.h"

namespace OpenTimer {

static void handler(AgentEvent* event) {

  auto agent = event->agent;
  
  // Debug connection.
  if(!OpenCluster::is_sockpeer_connected(event->fd())) {
    OpenCluster::LOGE("Cannot connect to master");
    event->reactor()->uncommit(event);
    return;
  }
  
  // Register the boundary pins.
  if(!agent->is_registered()) {
    printf("Register boundary pins\n");
    // Create a packet.
    Packet pkt;
    pkt.key = "register";
    for(auto item : agent->timer_ptr()->circuit_ptr()->primary_input_dict()) {
      pkt.value = pkt.value + "PI=" + item.second->name() + ";";
    }
    for(auto item : agent->timer_ptr()->circuit_ptr()->primary_output_dict()) {
      pkt.value = pkt.value + "PO=" + item.second->name() + ";";
    }

    pkt.size = sizeof(pkt.size) + pkt.key.size() + pkt.value.size() + 2;

    OpenCluster::LOGD(pkt.key, " ", pkt.value);

    (*event->ostream()) << pkt;
    agent->set_is_registered(true);
  }

  // Handling the write event.
  if(event->has_state(AgentEvent::EventState::ON_WRITE)) {

    OpenCluster::LOGD("agent write arrived (in_avail=", event->ostream()->rdbuf()->in_avail(), " bytes)");

    sleep(1);

  }
  // Handling the read event.
  else if(event->has_state(AgentEvent::EventState::ON_READ)) {
    
    OpenCluster::LOGD("agent read arrived (in_avail=", event->istream()->rdbuf()->in_avail(), " bytes)");

    while(1) {

      Packet pkt;

      *(event->istream()) >> pkt;
      
      if(pkt.size == 0) break;

      OpenCluster::LOGD("Received a new packet ", pkt.key, " / ", pkt.value, " (in_avail=", event->istream()->rdbuf()->in_avail(), ")");
      
      // ACTOR

      // key = repower_gate
      if(pkt.key == "repower_gate") {

        OpenCluster::LOGD("repower_gate ", pkt.key, " / ", pkt.value);

        stringstream ss(pkt.value);

        string gate;
        string cell;

        ss >> gate;
        ss >> cell;
      
        agent->timer_ptr()->repower_gate(gate, cell);

        OpenCluster::LOGD("Done ...");
      }
      else if(pkt.key == "update_timing") {

        OpenCluster::LOGD("update_timing ", pkt.key, " / ", pkt.value);
        
        // Parse the boundary timing
        // vlaue: out=1.2;in=2.0;
        const char delimiter[] = " =;";
        unique_ptr<char[]> value (new char[pkt.value.size() + 1]);
        memcpy(value.get(), pkt.value.c_str(), pkt.value.size() + 1);

        for(auto ptr = strtok(value.get(), delimiter); ptr; ptr = strtok(NULL, delimiter)) {
          string pin_name = ptr;
          float timing = atof(ptr=strtok(nullptr, delimiter));
          agent->timer_ptr()->set_at(pin_name, EARLY, RISE, timing);
        }
        
        // Update the timing.
        agent->timer_ptr()->update_timing();
        
        // Send the update boundary timing list
        Packet newpkt;
        newpkt.key = "update_boundary_timing";
        for(auto item : agent->timer_ptr()->circuit_ptr()->primary_output_dict()) {
          newpkt.value = newpkt.value + item.second->name() + "=" + to_string(agent->timer_ptr()->at(item.second->name(), EARLY, RISE)) + ";";
        }
    
        newpkt.size = sizeof(newpkt.size) + newpkt.key.size() + newpkt.value.size() + 2;
        
        OpenCluster::LOGD("Done... ", newpkt.key, " / ", newpkt.value);
        (*event->ostream()) << newpkt;
      }
      // Report tns command
      else if(pkt.key == "report_tns") {
        
        OpenCluster::LOGD("report_tns ", pkt.key, " / ", pkt.value);

        auto tns = agent->timer_ptr()->tns();
        Packet newpkt = make_packet("report_tns", std::to_string(tns));
        
        OpenCluster::LOGD("Done... ", newpkt.key, " / ", newpkt.value);
        *(event->ostream()) << newpkt;
      }
      // Unexpected command
      else {
        OpenCluster::LOGE("Undefined pkt key (", pkt.key, ")", 
                          "Please make sure the master has sent correct message");
      }
    }


    
    sleep(1);

  }
  else {
    OpenCluster::LOGW("Undefined state");
    sleep(1);
  }


}

// Constructor
Agent::Agent() :
  _timer_uptr { new Timer() } {
}

// Destructor.
Agent::~Agent() {

}


};  // End of namespace OpenTimer. ----------------------------------------------------------------

// Command:
//
// --hostname <string>
// --port <string>
// --set_verilog_fpath <string>
// --set_early_celllib_fpath <string>
// --set_late_celllib_fpath <string>
// --set_spef_fpath <string>
// --set_timing_fpath <string>
//
int main(int argc, char *argv[]) {
  
  // Initialize the logging.
  OpenTimer::Timer::init_logging(argv[0], 1);

  // Initialize the agent
  OpenTimer::Agent agent;
  int i(1);
  while(i < argc) {
    if(strcmp(argv[i], "--hostname") == 0) {
      agent.set_hostname(argv[++i]);
    }
    else if(strcmp(argv[i], "--port") == 0) {
      agent.set_port(argv[++i]);
    }
    else if(strcmp(argv[i], "--set_verilog_fpath") == 0) {
      agent.timer_ptr()->set_verilog_fpath(argv[++i]);
    }
    else if(strcmp(argv[i], "--set_early_celllib_fpath") == 0) {
      agent.timer_ptr()->set_early_celllib_fpath(argv[++i]);
    }
    else if(strcmp(argv[i], "--set_late_celllib_fpath") == 0) {
      agent.timer_ptr()->set_late_celllib_fpath(argv[++i]);
    }
    else if(strcmp(argv[i], "--set_spef_fpath") == 0) {
      agent.timer_ptr()->set_spef_fpath(argv[++i]);
    }
    else if(strcmp(argv[i], "--set_timing_fpath") == 0) {
      agent.timer_ptr()->set_timing_fpath(argv[++i]);
    }
    else {
      LOG(WARNING) << "Un-recognized command " << argv[i];
    }
    ++i;
  }

  // Initizlie the agent's timer.
  agent.timer_ptr()->init_timer();

  OpenCluster::Reactor<OpenTimer::AgentEvent> reactor;
  int connector = OpenCluster::make_socket_client(agent.hostname(), agent.port());
  auto event = reactor.insert_io_event(connector, OpenTimer::handler);
  event->agent = &agent;
  reactor.commit(event);

  reactor.dispatch();


  return 0;
}





