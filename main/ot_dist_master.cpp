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

#include "ot_dist_master.h"

namespace OpenTimer {

static int num_agents;
static string port;

// For simple benchmark
static const char* commands[] = {
  "repower_gate u3 INV_X1",
  "report_tns",
  nullptr
};

static int nth_command = 0;

// TODO:

// Format1:
// key: update_timing
// value: out=1.2;in=2.0;

// Format2:
// key: update_boundary_timing
// vlaue: out=1.2;in=2.0;

// Format3:
// key: report_tns
// vlaue: 29.9;


static void user_handler(MasterEvent* event) {

  auto master = event->master; 

  OpenCluster::LOGD("User's command coming");
  
  // TODO
  //if(master->num_registered_agents < num_agents) return;

  if(commands[nth_command] == nullptr) {
    OpenCluster::LOGI("All done");
    return;
  }

  if(master->is_locked()) return;

  // Handle the command.
  /*if(strstr(commands[nth_command] == "report_tns") == 0) {

    //
    master->set_is_locked(true);

    // update timing
    // Follow the topological order and issue the update_timing 
    // key= update_timing

    // 
  }

  // Assum this is design modifier
  else if( ... "repower_gate") {
    // broadcast 
  }
  else {
    OpenCluster::LOGE("Undefined command");
  }*/


  nth_command++;
}


static void client_handler(MasterEvent* event) {

  if(event->has_state(MasterEvent::ON_WRITE)) {
    
    OpenCluster::LOGD("agent write arrived (in_avail=", event->ostream()->rdbuf()->in_avail(), " bytes)");

    sleep(1);
  }
  else if(event->has_state(MasterEvent::ON_READ)) {
   
    OpenCluster::LOGD("agent read arrived (in_avail=", event->istream()->rdbuf()->in_avail(), " bytes)");

    while(1) {

      Packet pkt;

      *(event->istream()) >> pkt;
      
      if(pkt.size == 0) break;
      
      OpenCluster::LOGD(pkt.key, " ", pkt.key.size(), " ", pkt.value, " ", pkt.value.size());

      // Actor model.
      
      // TODO
      // key = register
      // Parse the PI/PO from pkt.value and construct the agent graph.
      if(pkt.key == "register") {
      }

      // An agent has just finished its update_timing
      else if(pkt.key == "update_boundary_timing") {

      }
      else if(pkt.key == "report_tns") {

      }
      else {

      }
    }

    
    sleep(1);
  }
  else {
    OpenCluster::LOGE("Undefined event state");
    
    sleep(1);
  }

}


// listener_handler
static void listener_handler(MasterEvent* event) {
  
  OpenCluster::LOGI("Listener event arrived");
  int newfd = OpenCluster::accept_socket_client(event->fd());
  OpenCluster::LOGI("Accepted a new connection (fd=", newfd, ")");
  auto new_client_event = event->reactor()->insert_io_event(newfd, client_handler);
  new_client_event->master = event->master;
  event->reactor()->commit(new_client_event);

  OpenCluster::LOGI("num events = ", event->reactor()->num_events());

  sleep(1);
}

};  // End of namespace OpenTimer. ----------------------------------------------------------------

// Command:
//
// --port <string>
//
int main(int argc, char *argv[]) {

  // Initialize the logging.
  OpenTimer::Timer::init_logging(argv[0], 1);

  if(argc != 5) {
    LOG(FATAL) << "Usage: ./OpenTimer-master --port <number> --num_agents <number>";
  }

  int i(1);
  while(i < argc) {
    if(strcmp(argv[i], "--num_agents") == 0) {
      OpenTimer::num_agents = atoi(argv[++i]);
    }
    else if(strcmp(argv[i], "--port") == 0) {
      OpenTimer::port = argv[++i];
    }
    else {
      LOG(WARNING) << "Un-recognized command " << argv[i];
    }
    ++i;
  }


  OpenTimer::Master master;
  
  OpenCluster::Reactor<OpenTimer::MasterEvent> reactor;
  int listener = OpenCluster::make_socket_server(OpenTimer::port);
  auto event = reactor.insert_io_event(listener, OpenTimer::listener_handler);
  event->master = &master;
  reactor.commit(event);

  event = reactor.insert_timeout_event(OpenCluster::make_duration<::std::chrono::seconds>(1), true, OpenTimer::user_handler);
  event->master = &master;
  reactor.commit(event);


  reactor.dispatch();


  return 0;
}





