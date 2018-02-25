/******************************************************************************
 *                                                                            *
 * Copyright (c) 2015, Tsung-Wei Huang and Martin D. F. Wong,                 *
 * University of Illinois at Urbana-Champaign (UIUC), IL, USA.                *
 *                                                                            *
 * All Rights Reserved.                                                       *
 *                                                                            *
 * This program is free software. You can redistribute and/or modify          *
 * it in accordance with the terms of the accompanying license agreement.     *
 * See LICENSE in the top-level directory for details.                        *
 *                                                                            *
 ******************************************************************************/

#include "ot_spef.h"

namespace OpenTimer {

// Constructor
SpefPin::SpefPin() {
}

// Destructor
SpefPin::~SpefPin() {
}

// Constructor
SpefCap::SpefCap():
  _cap(OT_DEFAULT_SPEFCAP)
{
}

// Destructor
SpefCap::~SpefCap() {
}

// Constructor
SpefRes::SpefRes():
  _res(OT_DEFAULT_SPEFRES)
{
}

// Destructor
SpefRes::~SpefRes() {
}

// Constructor
SpefNet::SpefNet():
  _lcap(OT_DEFAULT_SPEFLCAP),
  _spefpins_ptr(new spefpin_ptr_vt()),
  _spefcaps_ptr(new spefcap_ptr_vt()),
  _spefreses_ptr(new spefres_ptr_vt())
{
}

// Destructor
SpefNet::~SpefNet() {
  for(unsigned_t i=0; i<_spefpins_ptr->size(); i++ ) delete (*_spefpins_ptr) [i];
  for(unsigned_t i=0; i<_spefcaps_ptr->size(); i++ ) delete (*_spefcaps_ptr) [i];
  for(unsigned_t i=0; i<_spefreses_ptr->size(); i++) delete (*_spefreses_ptr)[i];
  delete _spefpins_ptr;
  delete _spefcaps_ptr;
  delete _spefreses_ptr;
}

// Function: insert_pin
spefpin_pt SpefNet::insert_spefpin(string_crt name) {
  spefpin_pt spefpin_ptr = new spefpin_t();
  spefpin_ptr->set_name(name);
  _spefpins_ptr->push_back(spefpin_ptr);
  return spefpin_ptr;
}

// Function: insert_cap
spefcap_pt SpefNet::insert_spefcap(string_crt name, float_ct cap) {
  spefcap_pt spefcap_ptr = new spefcap_t();
  spefcap_ptr->set_name(name);
  spefcap_ptr->set_cap(cap);
  _spefcaps_ptr->push_back(spefcap_ptr);
  return spefcap_ptr;
}

// Function: insert_res
spefres_pt SpefNet::insert_spefres(string_crt name_1, string_crt name_2, float_ct res) {
  spefres_pt spefres_ptr = new spefres_t();
  spefres_ptr->set_endpoint_name_1(name_1);
  spefres_ptr->set_endpoint_name_2(name_2);
  spefres_ptr->set_res(res);
  _spefreses_ptr->push_back(spefres_ptr);
  return spefres_ptr; 
}

// Constructor
Spef::Spef() {
  _spefnets_ptr = new spefnet_ptr_vt();
}

// Destructor
Spef::~Spef() {
  for(unsigned_t i=0; i<_spefnets_ptr->size(); i++) delete (*_spefnets_ptr)[i];
  delete _spefnets_ptr;
}

// Function: insert_spefnet
spefnet_pt Spef::insert_spefnet(string_crt name, float_ct lcap) {
  spefnet_pt spefnet_ptr = new spefnet_t();
  spefnet_ptr->set_name(name);
  spefnet_ptr->set_lcap(lcap);
  _spefnets_ptr->push_back(spefnet_ptr);
  return spefnet_ptr;
}

// Procedure: read_spef
// Read the parasitic file.
void_t Spef::read_spef(string_crt fpath) {

  // [BEG INFO]
  LOG(INFO) << string_t("Loading ") + Utility::realpath(fpath);
  // [END INFO]
  
  // Declare read/write tokens.
  char_pt prev_cursor;

  string_t token;
  string_t net_name;
  string_t pin_name;
  string_t numeric;
  string_t pin_dir;
  string_t name1, name2;
  
  // Declare the separators.
  char_t separators[] = {' ', '\n'};
  
  // Pointer to the spef net object.
  spefnet_pt spefnet_ptr(nullptr);
  
  // Declare freader.
  freader_t freader; 
  freader.read(fpath.c_str(), separators, sizeof(separators), nullptr, 0);
  
  while((token = freader.next_token()) != "") {
    
    // *D_NET section.
    if(token == "*D_NET") {

      net_name = freader.next_token();
      numeric = freader.next_token();
      spefnet_ptr = insert_spefnet(net_name, stof(numeric));
      
      // Read the section.
      while((token = freader.next_token()) != "") {

        // *CONN section. 
        if(token == "*CONN") {
          prev_cursor = freader.cursor();
          while((token = freader.next_token()) != "") {
            if(token[1] != 'P' && token[1] != 'I') {
              freader.set_cursor(prev_cursor);
              break;
            }
            pin_name = freader.next_token();
            pin_dir = freader.next_token();
            spefnet_ptr->insert_spefpin(pin_name); 
            prev_cursor = freader.cursor();
          }
        }
        // *CAP section.
        else if(token == "*CAP") {
          prev_cursor = freader.cursor();
          while((token = freader.next_token()) != "") {
            if(!isdigit(token[0])) {
              freader.set_cursor(prev_cursor);
              break;
            }
            name1 = freader.next_token();
            numeric = freader.next_token();
            spefnet_ptr->insert_spefcap(name1, stof(numeric));
            prev_cursor = freader.cursor();
          }
        }
        // *RES section.
        else if(token == "*RES") {
          prev_cursor = freader.cursor();
          while((token = freader.next_token()) != "") {
            if(!isdigit(token[0])) {
              freader.set_cursor(prev_cursor);
              break;
            }
            name1 = freader.next_token();
            name2 = freader.next_token();
            numeric = freader.next_token();
            spefnet_ptr->insert_spefres(name1, name2, stof(numeric));
            prev_cursor = freader.cursor();
          }
        }
        // *END section.
        else if(token == "*END") {
          break;
        }
        // Unexpected section keyword.
        else {
          LOG(WARNING) << string_t("Unexpected SPEF keywork \"") + token + "\"";
        }
      } 
    }
  }
}


};  // End of namespace OpenTimer. ----------------------------------------------------------------



