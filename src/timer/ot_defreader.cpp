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

#include "ot_defreader.h"

namespace OpenTimer {

// Constructor: DEFReader
DEFReader::DEFReader():
  _timer_ptr(nullptr)
{

}

// Destructor: ~DEFReader
DEFReader::~DEFReader() {

}

// Procedure: read
// Read the DEF file and initializes the data.
void_t DEFReader::read(timer_pt timer_ptr, string_crt fpath) {
  read(timer_ptr, fpath.c_str());
}

// Procedure: read
// Read the DEF file and initializes the data.
void_t DEFReader::read(timer_pt timer_ptr, char_cpt fpath) {

  CHECK(timer_ptr != nullptr);
  
  // [BEG INFO]
  LOG(INFO) <<  string_t("Loading ") + fpath;
  // [END INFO]

  _timer_ptr = timer_ptr;
  _fpath = fpath; 

  file_pt ifp = fopen(fpath, "r");
  
  CHECK(ifp != nullptr);
  
  // Initialize the reader.
  defrInit();

  // Set the callback functions.
  defrSetDieAreaCbk((defrBoxCbkFnType)die_area_cbk);
  defrSetComponentCbk(component_cbk);
  defrSetPinCbk((defrPinCbkFnType)pin_cbk);
  
  // Invoke the parser.
  defrRead(ifp, fpath, this, 1);

  fclose(ifp);
}

// Function: orientStr
char* DEFReader::orientStr(int orient) {
  switch (orient) {
      case 0: return ((char*)"N");
      case 1: return ((char*)"W");
      case 2: return ((char*)"S");
      case 3: return ((char*)"E");
      case 4: return ((char*)"FN");
      case 5: return ((char*)"FW");
      case 6: return ((char*)"FS");
      case 7: return ((char*)"FE");
  };
  return ((char*)"BOGUS");
}

// Function: orient_enum
orientation_e DEFReader::orientation_enum(int orient) {
  switch (orient) {
    case 0: return N_ORIENTATION; break;
    case 1: return W_ORIENTATION; break;
    case 2: return S_ORIENTATION; break;
    case 3: return E_ORIENTATION; break;
    case 4: return FN_ORIENTATION; break;
    case 5: return FW_ORIENTATION; break;
    case 6: return FS_ORIENTATION; break;
    case 7: return FE_ORIENTATION; break;
    default: return UNDEFINED_ORIENTATION; break;
  };
}

// Function: die_area_cbk
// Callback function for the die area.
//
// [DIEAREA pt pt [pt] ... ;]
//
int DEFReader::die_area_cbk(defrCallbackType_e c, void* cl, void* ud) {

  CHECK(c == defrDieAreaCbkType);
  
  DEFReader* def_ptr = (DEFReader*)ud;
  
  defiBox* box((defiBox*)cl);

  #ifdef DEF_CALLBACK_VERBOSITY
  printf("DIEAREA %d %d %d %d ;\n", box->xl(), box->yl(), box->xh(), box->yh());
  #endif

  point_t llp(box->xl(), box->yl());
  point_t urp(box->xh(), box->yh());
  def_ptr->timer_ptr()->circuit_ptr()->set_die(llp, urp);

  //struct defiPoints points;
  //printf("DIEAREA ");
  //points = box->getPoint();
  //for(int i = 0; i < points.numPoints; i++) {
  //  printf("%d %d ", points.x[i], points.y[i]);
  //}
  //printf(";\n");
  return 0;
}

// Function: component_cbk
// Callback function for the component.
//
// COMPONENTS numComps ;
// [- compName modelName
//   [+ EEQMASTER macroName]
//   [+ SOURCE {NETLIST | DIST | USER | TIMING}]
//   [+ {FIXED pt orient | COVER pt orient | PLACED pt orient
//      | UNPLACED} ]
//   [+ MASKSHIFT shiftLayerMasks]
//   [+ HALO [SOFT] left bottom right top]
//   [+ ROUTEHALO haloDist minLayer maxLayer]
//   [+ WEIGHT weight]
//   [+ REGION regionName]
//   [+ PROPERTY {propName propVal} ...]...
// ;] ...
// END COMPONENTS
//
int DEFReader::component_cbk(defrCallbackType_e c, defiComponent* co, void* ud) {
  
  CHECK(c == defrComponentCbkType);

  DEFReader* def_ptr = (DEFReader*)ud;

  // Specifies the component name in the design, which is an instance of modelName, the name of 
  // a model defined in the library. A modelName must be specified with each compName.
  // Then, query the pointer to the gate from the timer and insert a new gate if the gate doesn't 
  // exist in the current design.
  #ifdef DEF_CALLBACK_VERBOSITY
  printf("- %s %s ", co->id(), co->name());
  #endif
  
  gate_pt gate_ptr = def_ptr->timer_ptr()->gate_ptr(co->id());
  if(gate_ptr == nullptr) {
    gate_ptr = def_ptr->timer_ptr()->insert_gate(co->id(), co->name());
  }

  // co->changeIdAndName("idName", "modelName");
  // printf("%s %s ", co->id(), co->name());
  if (co->hasNets()) {
    for(int i = 0; i < co->numNets(); i++)
      printf("%s ", co->net(i));
  }

  // Specifies that the component has a location and cannot be moved by automatic tools, but can 
  // be moved using interactive commands. You must specify the component's location and orientation.
  if (co->isFixed()) {
    #ifdef DEF_CALLBACK_VERBOSITY
    printf("+ FIXED %d %d %s ", co->placementX(), co->placementY(), co->placementOrientStr());
    #endif
    gate_ptr->set_mobility(FIXED_MOBILITY);
    gate_ptr->set_location(co->placementX(), co->placementY());
    gate_ptr->set_orientation(DEFReader::orientation_enum(co->placementOrient()));
  }
  // Specifies that the component has a location and is a part of a cover macro. A COVER component 
  // cannot be moved by automatic tools or interactive commands. You must specify the component's 
  // location and its orientation.
  if (co->isCover()) {
    #ifdef DEF_CALLBACK_VERBOSITY
    printf("+ COVER %d %d %s ", co->placementX(), co->placementY(), DEFReader::orientStr(co->placementOrient()));
    #endif
    gate_ptr->set_mobility(COVER_MOBILITY);
    gate_ptr->set_location(co->placementX(), co->placementY());
    gate_ptr->set_orientation(DEFReader::orientation_enum(co->placementOrient()));
  }
  // Specifies that the component has a location, but can be moved using automatic layout tools. 
  // You must specify the component's location and orientation.
  if (co->isPlaced()) {
    #ifdef DEF_CALLBACK_VERBOSITY
    printf("+ PLACED %d %d %s ", co->placementX(), co->placementY(), DEFReader::orientStr(co->placementOrient()));
    #endif
    gate_ptr->set_mobility(PLACED_MOBILITY);
    gate_ptr->set_location(co->placementX(), co->placementY());
    gate_ptr->set_orientation(DEFReader::orientation_enum(co->placementOrient()));
  }
  // Specifies that the component doesn't have a location.
  if (co->isUnplaced()) {
    #ifdef DEF_CALLBACK_VERBOSITY
    printf("+ UNPLACED ");
    #endif
    gate_ptr->set_mobility(UNPLACED_MOBILITY);
    if ((co->placementX() != -1) || (co->placementY() != -1)) {
      #ifdef DEF_CALLBACK_VERBOSITY
      printf("%d %d %s ", co->placementX(), co->placementY(), DEFReader::orientStr(co->placementOrient()));
      #endif
      gate_ptr->set_location(co->placementX(), co->placementY());
      gate_ptr->set_orientation(DEFReader::orientation_enum(co->placementOrient()));
    }
  }

  // Specifies the source of the gate.
  // DIST   : Component is a physical component (that is, it only connects to power or ground 
  //          nets), such as filler cells, well-taps, and decoupling caps.
  // NETLIST: Component is specified in the original netlist. This is the default value, and is 
  //          normally not written out in the DEF file.
  // TIMING : Component is a logical rather than physical change to the netlist, and is typically 
  //          used as a buffer for a clock-tree, or to improve timing on long nets.
  // USER   : Component is generated by the user for some user-defined reason.
  if (co->hasSource()) {
    #ifdef DEF_CALLBACK_VERBOSITY
    printf("+ SOURCE %s ", co->source());
    #endif
    if(strcmp(co->source(), "NETLIST") == 0) {
      gate_ptr->set_source(NETLIST_GATE_SOURCE);
    }
    else if(strcmp(co->source(), "DIST") == 0) {
      gate_ptr->set_source(DIST_GATE_SOURCE);
    }
    else if(strcmp(co->source(), "USER") == 0) {
      gate_ptr->set_source(USER_GATE_SOURCE);
    }
    else if(strcmp(co->source(), "TIMING") == 0) {
      gate_ptr->set_source(TIMING_GATE_SOURCE);
    }
    else {
      CHECK(false);
    }
  }
  
  // Specifies the weight of the component, which determines whether or not automatic placement 
  // attempts to keep the component near the specified location. weight is only meaningful when 
  // the component is placed. All non-zero weights have the same effect during automatic placement.
  // Default: 0 
  if (co->hasWeight()) {
    #ifdef DEF_CALLBACK_VERBOSITY
    printf("+ WEIGHT %d ", co->weight());
    #endif
    gate_ptr->set_weight(co->weight());
  }
  
  // BEG TODO--------------------------------------------------------------------------------------  
  if (co->hasGenerate()) {
    printf("+ GENERATE %s ", co->generateName());
    if (co->macroName() && *(co->macroName()))
      printf("%s ", co->macroName());
  }

  if (co->hasEEQ())
    printf("+ EEQMASTER %s ", co->EEQ());
  if (co->hasRegionName())
    printf("+ REGION %s ", co->regionName());
  if (co->hasRegionBounds()) {
    int *xl, *yl, *xh, *yh;
    int size;
    co->regionBounds(&size, &xl, &yl, &xh, &yh);
    for (int i = 0; i < size; i++) { 
      printf("+ REGION %d %d %d %d \n", xl[i], yl[i], xh[i], yh[i]);
    }
  }
  if (co->maskShiftSize()) {
    printf("+ MASKSHIFT ");
    for (int i = co->maskShiftSize()-1; i >= 0; i--) {
      printf("%d", co->maskShift(i));
    }
    printf("\n");
  }
  if (co->hasHalo()) {
    int left, bottom, right, top;
    (void) co->haloEdges(&left, &bottom, &right, &top);
    printf("+ HALO ");
    if (co->hasHaloSoft())
      printf("SOFT ");
      printf("%d %d %d %d\n", left, bottom, right, top);
  }
  if (co->hasRouteHalo()) {
    printf("+ ROUTEHALO %d %s %s\n", co->haloDist(), co->minLayer(), co->maxLayer());
  }
  if (co->hasForeignName()) {
    printf("+ FOREIGN %s %d %d %s %d ", co->foreignName(), co->foreignX(),
                                        co->foreignY(), co->foreignOri(),
                                        co->foreignOrient());
  }
  if (co->numProps()) {
    for(int i = 0; i < co->numProps(); i++) {
      printf("+ PROPERTY %s %s ", co->propName(i), co->propValue(i));
      switch (co->propType(i)) {
        case 'R': printf("REAL ");
        break;
        case 'I': printf("INTEGER ");
        break;
        case 'S': printf("STRING ");
        break;
        case 'Q': printf("QUOTESTRING ");
        break;
        case 'N': printf("NUMBER ");
        break;
      }
    }
  }
  #ifdef DEF_CALLBACK_VERBOSITY
  printf(";\n");
  #endif
  // END TODO--------------------------------------------------------------------------------------

  //--numObjs;
  //if (numObjs <= 0)
  //printf("END COMPONENTS\n");

  return 0;
}

// Function: pin_cbk
// Defines external pins. Each pin definition assigns a pin name for the external pin and 
// associates the pin name with a corresponding internal net name. The pin name and the net name 
// can be the same.
// When the design is a chip rather than a block, the PINS statement describes logical pins, without 
// placement or physical information.
//
// [PINS numPins ;
// [ [- pinName + NET netName]
//      [+ SPECIAL]
//      [+ DIRECTION {INPUT | OUTPUT | INOUT | FEEDTHRU}]
//      [+ NETEXPR "netExprPropName defaultNetName"]
//      [+ SUPPLYSENSITIVITY powerPinName]
//      [+ GROUNDSENSITIVITY groundPinName]
//      [+ USE {SIGNAL | POWER | GROUND | CLOCK | TIEOFF | ANALOG | SCAN | RESET}]
//      [+ ANTENNAPINPARTIALMETALAREA value [LAYER layerName]] ...
//      [+ ANTENNAPINPARTIALMETALSIDEAREA value [LAYER layerName]] ...
//      [+ ANTENNAPINPARTIALCUTAREA value [LAYER layerName]] ...
//      [+ ANTENNAPINDIFFAREA value [LAYER layerName]] ...
//      [+ ANTENNAMODEL {OXIDE1 | OXIDE2 | OXIDE3 | OXIDE4}] ...
//      [+ ANTENNAPINGATEAREA value [LAYER layerName]] ...
//      [+ ANTENNAPINMAXAREACAR value LAYER layerName] ...
//      [+ ANTENNAPINMAXSIDEAREACAR value LAYER layerName] ...
//      [+ ANTENNAPINMAXCUTCAR value LAYER layerName] ...
//      [[+ PORT] 
//       [+ LAYER layerName 
//            [MASK maskNum]
//            [SPACING minSpacing | DESIGNRULEWIDTH effectiveWidth] pt pt 
//       |+ POLYGON layerName 
//            [MASK maskNum] 
//            [SPACING minSpacing | DESIGNRULEWIDTH effectiveWidth] pt pt pt ...
//       |+ VIA viaName 
//            [MASK viaMaskNu] pt] ...
//       [+ COVER pt orient | FIXED pt orient | PLACED pt orient]
//      ]... 
// ;] ...
// END PINS]
//
int DEFReader::pin_cbk(defrCallbackType_e c, void* cl, void* ud) {
  
  CHECK(defrPinCbkType == c);
  
  DEFReader* def_ptr = (DEFReader*)ud;

  defiPin* pin;
  int i, j, k;
  int xl, yl, xh, yh;
  defiPinAntennaModel* aModel;

  pin = (defiPin*)cl;

  // Specifies the name for the external pin, and the corresponding internal net 
  //(defined in NETS or SPECIALNETS statements). 
  #ifdef DEF_CALLBACK_VERBOSITY
  printf( "- %s + NET %s ", pin->pinName(), pin->netName());
  #endif
  
  // pin->changePinName("pinName");
  // printf( "%s ", pin->pinName());
  if (pin->hasDirection()) {
    #ifdef DEF_CALLBACK_VERBOSITY
    printf( "+ DIRECTION %s ", pin->direction());
    #endif
  }
  
  // Query the pin pointer and insert the port if the corresponding pin doesn't exist
  // in the current design.
  pin_pt pin_ptr = def_ptr->timer_ptr()->pin_ptr(pin->pinName());
  if(pin_ptr == nullptr) {
    // Primary input.
    if(strcmp(pin->direction(), "INPUT") == 0) {
      pin_ptr = def_ptr->timer_ptr()->insert_primary_input(pin->pinName())->pin_ptr();
    }
    // Primary output.
    else if(strcmp(pin->direction(), "OUTPUT") == 0) {
      pin_ptr = def_ptr->timer_ptr()->insert_primary_output(pin->pinName())->pin_ptr();
    }
    // Unexpected behavior.
    else {
      CHECK(false);
    }
  }

  // Query the use type of the pin, which could be one of SIGNAL, POWER, GROUND, CLOCK, TIEOFF, 
  // ANALOG, SCAN, and RESET.
  if (pin->hasUse()) {
    #ifndef DEF_CALLBACK_VERBOSITY
    printf( "+ USE %s ", pin->use());
    #endif

    if(strcmp(pin->use(), "SIGNAL") == 0) {
      pin_ptr->set_use(SIGNAL_PIN_USE);
    }
    else if(strcmp(pin->use(), "POWER") == 0) {
      pin_ptr->set_use(POWER_PIN_USE);
    }
    else if(strcmp(pin->use(), "GROUND") == 0) {
      pin_ptr->set_use(GROUND_PIN_USE);
    }
    else if(strcmp(pin->use(), "CLOCK") == 0) {
      pin_ptr->set_use(CLOCK_PIN_USE);
    }
    else if(strcmp(pin->use(), "TIEOFF") == 0) {
      pin_ptr->set_use(TIEOFF_PIN_USE);
    }
    else if(strcmp(pin->use(), "ANALOG") == 0) {
      pin_ptr->set_use(ANALOG_PIN_USE);
    }
    else if(strcmp(pin->use(), "SCAN") == 0) {
      pin_ptr->set_use(SCAN_PIN_USE);
    }
    else if(strcmp(pin->use(), "RESET") == 0) {
      pin_ptr->set_use(RESET_PIN_USE);
    }
    else {
      CHECK(false);
    }
  }
  
  // BEG TODO--------------------------------------------------------------------------------------
  if (pin->hasNetExpr()) {
    printf( "+ NETEXPR \"%s\" ", pin->netExpr());
  }
  if (pin->hasSupplySensitivity()) {
    printf( "+ SUPPLYSENSITIVITY %s ", pin->supplySensitivity());
  }
  if (pin->hasGroundSensitivity()) {
    printf( "+ GROUNDSENSITIVITY %s ", pin->groundSensitivity());
  }

  if (pin->hasLayer()) {
    struct defiPoints points;
    for (i = 0; i < pin->numLayer(); i++) {
      #ifdef DEF_CALLBACK_VERBOSITY
      printf( "\n  + LAYER %s ", pin->layer(i));
      #endif
      if (pin->layerMask(i)) {
        printf( "MASK %d ", pin->layerMask(i));
      }
      if (pin->hasLayerSpacing(i)) {
        printf( "SPACING %d ", pin->layerSpacing(i));
      }
      if (pin->hasLayerDesignRuleWidth(i)) {
        printf( "DESIGNRULEWIDTH %d ", pin->layerDesignRuleWidth(i));
      }
      pin->bounds(i, &xl, &yl, &xh, &yh);
      #ifdef DEF_CALLBACK_VERBOSITY
      printf( "%d %d %d %d ", xl, yl, xh, yh);
      #endif
    }

    for (i = 0; i < pin->numPolygons(); i++) {
      printf( "\n  + POLYGON %s ", pin->polygonName(i));
      if (pin->polygonMask(i))
        printf( "MASK %d ", pin->polygonMask(i));
      if (pin->hasPolygonSpacing(i))
        printf( "SPACING %d ", pin->polygonSpacing(i));
      if (pin->hasPolygonDesignRuleWidth(i))
        printf( "DESIGNRULEWIDTH %d ", pin->polygonDesignRuleWidth(i));
      points = pin->getPolygon(i);
      for (j = 0; j < points.numPoints; j++)
        printf( "%d %d ", points.x[j], points.y[j]);
    }
    for (i = 0; i < pin->numVias(); i++) {
      if (pin->viaTopMask(i) || pin->viaCutMask(i) || pin->viaBottomMask(i)) {
        printf( "\n  + VIA %s MASK %d%d%d %d %d ", pin->viaName(i),
                                                   pin->viaTopMask(i),
                                                   pin->viaCutMask(i),
                                                   pin->viaBottomMask(i),
                                                   pin->viaPtX(i), 
                                                   pin->viaPtY(i));
      } 
      else {
        printf( "\n  + VIA %s %d %d ", pin->viaName(i), pin->viaPtX(i), pin->viaPtY(i));
      }
    }
  }

  if (pin->hasPort()) {
    struct defiPoints points;
    defiPinPort* port;
    for (j = 0; j < pin->numPorts(); j++) {
      port = pin->pinPort(j);
      printf( "\n  + PORT");
      for (i = 0; i < port->numLayer(); i++) {
        printf( "\n     + LAYER %s ", port->layer(i));
        if (port->layerMask(i))
          printf( "MASK %d ", port->layerMask(i));
        if (port->hasLayerSpacing(i))
          printf( "SPACING %d ", port->layerSpacing(i));
        if (port->hasLayerDesignRuleWidth(i))
          printf( "DESIGNRULEWIDTH %d ", port->layerDesignRuleWidth(i));
        port->bounds(i, &xl, &yl, &xh, &yh);
        printf( "%d %d %d %d ", xl, yl, xh, yh);
      }
      for (i = 0; i < port->numPolygons(); i++) {
        printf( "\n     + POLYGON %s ", port->polygonName(i));
        if (port->polygonMask(i))
          printf( "MASK %d ", port->polygonMask(i));
        if (port->hasPolygonSpacing(i))
          printf( "SPACING %d ", port->polygonSpacing(i));
        if (port->hasPolygonDesignRuleWidth(i))
          printf( "DESIGNRULEWIDTH %d ", port->polygonDesignRuleWidth(i));
        points = port->getPolygon(i);
        for (k = 0; k < points.numPoints; k++)
        printf( "( %d %d ) ", points.x[k], points.y[k]);
      }
      for (i = 0; i < port->numVias(); i++) {
        if (port->viaTopMask(i) || port->viaCutMask(i) || port->viaBottomMask(i)) {
          printf( "\n     + VIA %s MASK %d%d%d ( %d %d ) ",
                            port->viaName(i),
                            port->viaTopMask(i),
                            port->viaCutMask(i),
                            port->viaBottomMask(i),
                            port->viaPtX(i),
                            port->viaPtY(i));
        } else {
          printf( "\n     + VIA %s ( %d %d ) ", port->viaName(i), port->viaPtX(i), port->viaPtY(i));
        }
      }
      if (port->hasPlacement()) {
        if (port->isPlaced()) {
          printf( "\n     + PLACED ");
          printf( "( %d %d ) %s ", port->placementX(), port->placementY(), orientStr(port->orient()));
        }
        if (port->isCover()) {
          printf( "\n     + COVER ");
          printf( "( %d %d ) %s ", port->placementX(), port->placementY(), orientStr(port->orient()));
        }
        if (port->isFixed()) {
          printf( "\n     + FIXED ");
          printf( "( %d %d ) %s ", port->placementX(), port->placementY(), orientStr(port->orient()));
        }
      }
    }
  }
  // END TODO--------------------------------------------------------------------------------------
  
  // The pin has a physical location in the placement.
  if (pin->hasPlacement()) {
    if (pin->isPlaced()) {
      #ifdef DEF_CALLBACK_VERBOSITY
      printf( "+ PLACED ");
      printf( "( %d %d ) %s ", pin->placementX(), pin->placementY(), orientStr(pin->orient()));
      #endif
      pin_ptr->set_mobility(PLACED_MOBILITY);
      pin_ptr->set_location(pin->placementX(), pin->placementY());
      pin_ptr->set_orientation(DEFReader::orientation_enum(pin->orient()));
    }
    // Specifies the pin's location, orientation, and that it is a part of the cover macro. 
    // A COVER pin cannot be moved by automatic tools or by interactive commands. If you specify a 
    // placement status for a pin, you must also include a LAYER statement.
    if (pin->isCover()) {
      #ifdef DEF_CALLBACK_VERBOSITY
      printf( "+ COVER ");
      printf( "( %d %d ) %s ", pin->placementX(), pin->placementY(), orientStr(pin->orient()));
      #endif
      pin_ptr->set_mobility(COVER_MOBILITY);
      pin_ptr->set_location(pin->placementX(), pin->placementY());
      pin_ptr->set_orientation(DEFReader::orientation_enum(pin->orient()));
    }
    // Specifies the pin's location, orientation, and that it's location cannot be moved by 
    // automatic tools, but can be moved by interactive commands. If you specify a placement 
    // status for a pin, you must also include a LAYER statement.
    if (pin->isFixed()) {
      #ifdef DEF_CALLBACK_VERBOSITY
      printf( "+ FIXED ");
      printf( "( %d %d ) %s ", pin->placementX(), pin->placementY(), orientStr(pin->orient()));
      #endif
      pin_ptr->set_mobility(FIXED_MOBILITY);
      pin_ptr->set_location(pin->placementX(), pin->placementY());
      pin_ptr->set_orientation(DEFReader::orientation_enum(pin->orient()));
    }
    // The pin is not placed yet and it doesn't have a placement location.
    if (pin->isUnplaced()) {
      #ifdef DEF_CALLBACK_VERBOSITY
      printf( "+ UNPLACED ");
      #endif
      pin_ptr->set_mobility(UNPLACED_MOBILITY);
    }
  }
  
  //BEG TODO------------------------------------------------------------------------------------------
  if (pin->hasSpecial()) {
    printf( "+ SPECIAL ");
  }
         if (pin->hasAPinPartialMetalArea()) {
             for (i = 0; i < pin->numAPinPartialMetalArea(); i++) {
                printf( "ANTENNAPINPARTIALMETALAREA %d",
                        pin->APinPartialMetalArea(i));
                if (*(pin->APinPartialMetalAreaLayer(i)))
                    printf( " LAYER %s",
                            pin->APinPartialMetalAreaLayer(i));
                printf( "\n");
             }
         }
         if (pin->hasAPinPartialMetalSideArea()) {
             for (i = 0; i < pin->numAPinPartialMetalSideArea(); i++) {
                printf( "ANTENNAPINPARTIALMETALSIDEAREA %d",
                        pin->APinPartialMetalSideArea(i));
                if (*(pin->APinPartialMetalSideAreaLayer(i)))
                    printf( " LAYER %s",
                        pin->APinPartialMetalSideAreaLayer(i));
                printf( "\n");
             }
         }
         if (pin->hasAPinDiffArea()) {
             for (i = 0; i < pin->numAPinDiffArea(); i++) {
                printf( "ANTENNAPINDIFFAREA %d", pin->APinDiffArea(i));
                if (*(pin->APinDiffAreaLayer(i)))
                    printf( " LAYER %s", pin->APinDiffAreaLayer(i));
                printf( "\n");
             }
         }
         if (pin->hasAPinPartialCutArea()) {
             for (i = 0; i < pin->numAPinPartialCutArea(); i++) {
                printf( "ANTENNAPINPARTIALCUTAREA %d",
                        pin->APinPartialCutArea(i));
                if (*(pin->APinPartialCutAreaLayer(i)))
                    printf( " LAYER %s", pin->APinPartialCutAreaLayer(i));
                printf( "\n");
             }
         }

         for (j = 0; j < pin->numAntennaModel(); j++) {
            aModel = pin->antennaModel(j);
 
            printf( "ANTENNAMODEL %s\n",
                    aModel->antennaOxide()); 
 
            if (aModel->hasAPinGateArea()) {
                for (i = 0; i < aModel->numAPinGateArea();
                   i++) {
                   printf( "ANTENNAPINGATEAREA %d",
                           aModel->APinGateArea(i));
                   if (aModel->hasAPinGateAreaLayer(i))
                       printf( " LAYER %s", aModel->APinGateAreaLayer(i));
                   printf( "\n");
                }
            }
            if (aModel->hasAPinMaxAreaCar()) {
                for (i = 0;
                   i < aModel->numAPinMaxAreaCar(); i++) {
                   printf( "ANTENNAPINMAXAREACAR %d",
                           aModel->APinMaxAreaCar(i));
                   if (aModel->hasAPinMaxAreaCarLayer(i))
                       printf(
                           " LAYER %s", aModel->APinMaxAreaCarLayer(i));
                   printf( "\n");
                }
            }
            if (aModel->hasAPinMaxSideAreaCar()) {
                for (i = 0;
                     i < aModel->numAPinMaxSideAreaCar(); 
                     i++) {
                   printf( "ANTENNAPINMAXSIDEAREACAR %d",
                           aModel->APinMaxSideAreaCar(i));
                   if (aModel->hasAPinMaxSideAreaCarLayer(i))
                       printf(
                           " LAYER %s", aModel->APinMaxSideAreaCarLayer(i));
                   printf( "\n");
                }
            }
            if (aModel->hasAPinMaxCutCar()) {
                for (i = 0; i < aModel->numAPinMaxCutCar();
                   i++) {
                   printf( "ANTENNAPINMAXCUTCAR %d",
                       aModel->APinMaxCutCar(i));
                   if (aModel->hasAPinMaxCutCarLayer(i))
                       printf( " LAYER %s",
                       aModel->APinMaxCutCarLayer(i));
                   printf( "\n");
                }
            }
         }
  // END TODO--------------------------------------------------------------------------------------

  #ifdef DEF_CALLBACK_VERBOSITY
  printf( ";\n");
  #endif
  //--numObjs;
  //if (numObjs <= 0)
  //    printf( "END PINS\n");
  return 0;
}

};

