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

#include "ot_lefreader.h"

namespace OpenTimer {

// Constructor: LEFReader
LEFReader::LEFReader():
  _timer_ptr(nullptr),
  _site_cursor(nullptr),                              
  _macro_cursor(nullptr),                                    
  _layer_cursor(nullptr),
  _macropin_cursor(nullptr),                 
  _layer_geometry_cursor(nullptr),                     
  _parse65nm(false),
  _parseLef58Type(false)
{
}

// Destructor: ~LEF
LEFReader::~LEFReader() {

}

// Procedure: read
// Read the LEF file and initialize the data into the circuit. The procedure mainly initialize 
// the LEF parser routine, including the setting the call back functions.
void_t LEFReader::read(timer_pt timer_ptr, string_crt fpath) {
  read(timer_ptr, fpath.c_str());
}

// Procedure: read
// Read the LEF file and initialize the data into the circuit. The procedure mainly initialize 
// the LEF parser routine, including the setting the call back functions.
void_t LEFReader::read(timer_pt timer_ptr, char_cpt fpath) {

  CHECK(timer_ptr != nullptr);
  
  // [BEG INFO]
  LOG(INFO) << string_t("Loading ") + fpath;
  // [END INFO]

  _timer_ptr = timer_ptr;
  _fpath = fpath;
  
  file_pt ifp = fopen(fpath, "r");
  
  CHECK(ifp != nullptr);

  // Initialize the reader.
  lefrInit();

  // Set the callback functions.
  lefrSetUnitsCbk(units_cbk);
  lefrSetSiteCbk(site_cbk);
  lefrSetLayerCbk(layer_cbk);
  lefrSetPinCbk(pin_cbk);
  lefrSetMacroBeginCbk(macro_begin_cbk);
  lefrSetMacroCbk(macro_cbk);
  lefrSetMacroEndCbk(macro_end_cbk);
  
  // Invoke the parser.
  lefrRead(ifp, fpath, this);

  fclose(ifp);
}

// Function: chkNum
// This function is added due to the rounding between machines are
// different.  For a 5, solaries will round down while hppa will roundup.
// This function will make sure it round up for all the machine
double LEFReader::chkNum(double num) {
  long tempNum;
  if ((num > 1000004) || (num < -1000004)) {
     tempNum = (long)num;
     if ((tempNum%5) == 0)
        return num + 3;
  }
  return num;
}

// Function: orientStr
char* LEFReader::orientStr(int orient) {
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

// Function: prtGeometry
// { LAYER layerName
//   [EXCEPTPGNET]
//   [SPACING minSpacing | DESIGNRULEWIDTH value] ;
//   [WIDTH width ;]
//   { PATH [MASK maskNum] pt ... ;
//   | PATH [MASK maskNum] ITERATE pt ... stepPattern ;
//   | RECT [MASK maskNum] pt pt ;
//   | RECT [MASK maskNum] ITERATE pt pt stepPattern ;
//   | POLYGON [MASK maskNum] pt pt pt pt ... ; 
//   | POLYGON [MASK maskNum] ITERATE pt pt pt pt ... stepPattern ;
//   } ...
//   | VIA [MASK viaMaskNum] pt viaName ; 
//   | VIA ITERATE [MASK viaMaskNum] pt viaName stepPattern ;
// } ...
void LEFReader::prtGeometry(lefiGeometries *geometry) {
  
  CHECK(geometry != nullptr);

  // TODO : insert a geometry here.
  //macropin_cursor();
  
  int numItems = geometry->numItems();
  int i, j;
  lefiGeomPath        *path;
  lefiGeomPathIter    *pathIter;
  lefiGeomRect        *rect;
  lefiGeomRectIter    *rectIter;
  lefiGeomPolygon     *polygon;
  lefiGeomPolygonIter *polygonIter;
  lefiGeomVia         *via;
  lefiGeomViaIter     *viaIter;

  for (i = 0; i < numItems; i++) {
    switch (geometry->itemType(i)) {
      case  lefiGeomClassE:
        printf("CLASS %s ", geometry->getClass(i));
      break;
      case lefiGeomLayerE: 
        #ifdef LEF_CALLBACK_VERBOSITY
        printf("      LAYER %s ;\n", geometry->getLayer(i));
        #endif 
        _layer_geometry_cursor = macropin_cursor()->insert_layer_geometry(geometry->getLayer(i));
      break;
      case lefiGeomLayerExceptPgNetE:
        printf("      EXCEPTPGNET ;\n");
      break;
      case lefiGeomLayerMinSpacingE:
        #ifdef LEF_CALLBACK_VERBOSITY
        printf("      SPACING %g ;\n", geometry->getLayerMinSpacing(i));
        #endif
        CHECK(layer_geometry_cursor() != nullptr);
        layer_geometry_cursor()->set_spacing(geometry->getLayerMinSpacing(i));
      break;
      case lefiGeomLayerRuleWidthE:
        printf("      DESIGNRULEWIDTH %g ;\n", geometry->getLayerRuleWidth(i));
      break;
      case lefiGeomWidthE:
        #ifdef LEF_CALLBACK_VERBOSITY
        printf("      WIDTH %g ;\n", geometry->getWidth(i));
        #endif 
        CHECK(layer_geometry_cursor() != nullptr);
        layer_geometry_cursor()->set_width(geometry->getWidth(i));
      break;
      case lefiGeomPathE:
        path = geometry->getPath(i);
        if (path->colorMask != 0) {
          printf("      PATH MASK %d ", path->colorMask);
        } else {
          printf("      PATH ");
        }
        for (j = 0; j < path->numPoints; j++) {
          if (j + 1 == path->numPoints) // last one on the list
            printf("      ( %g %g ) ;\n", path->x[j], path->y[j]);
          else
            printf("      ( %g %g )\n", path->x[j], path->y[j]);
        }
      break;
        case lefiGeomPathIterE:
            pathIter = geometry->getPathIter(i);
            if (pathIter->colorMask != 0) {
                printf("      PATH MASK %d ITERATED ", pathIter->colorMask);
            } else {
                printf("      PATH ITERATED ");
            }
            for (j = 0; j < pathIter->numPoints; j++)
                printf("      ( %g %g )\n", pathIter->x[j],
                        pathIter->y[j]);
            printf("      DO %g BY %g STEP %g %g ;\n", pathIter->xStart,
                    pathIter->yStart, pathIter->xStep, pathIter->yStep);
            break;
      case lefiGeomRectE:
        rect = geometry->getRect(i);
        if (rect->colorMask != 0) {
          printf("      RECT MASK %d ( %f %f ) ( %f %f ) ;\n", rect->colorMask, rect->xl, rect->yl, rect->xh, rect->yh);
        } else {
          #ifdef LEF_CALLBACK_VERBOSITY
          printf("      RECT ( %f %f ) ( %f %f ) ;\n", rect->xl, rect->yl, rect->xh, rect->yh);
          #endif
          CHECK(layer_geometry_cursor() != nullptr);
          layer_geometry_cursor()->insert_rectangle(rect->xl, rect->yl, rect->xh, rect->yh);
        }
      break;
      case lefiGeomRectIterE:
        rectIter = geometry->getRectIter(i);
        if (rectIter->colorMask != 0) {
          printf("      RECT MASK %d ITERATE ( %f %f ) ( %f %f )\n",
                        rectIter->colorMask,
                        rectIter->xl, rectIter->yl, rectIter->xh, rectIter->yh);
        } else {
          printf("      RECT ITERATE ( %f %f ) ( %f %f )\n",
                        rectIter->xl, rectIter->yl, rectIter->xh, rectIter->yh);
        }
        printf("      DO %g BY %g STEP %g %g ;\n",
                    rectIter->xStart, rectIter->yStart, rectIter->xStep,
                    rectIter->yStep);
      break;
      case lefiGeomPolygonE:
        polygon = geometry->getPolygon(i);
        if (polygon->colorMask != 0) {
          printf("      POLYGON MASK %d ", polygon->colorMask);
        } else {
          printf("      POLYGON ");
        }
        for (j = 0; j < polygon->numPoints; j++) {
          if (j + 1 == polygon->numPoints) // last one on the list
            printf("      ( %g %g ) ;\n", polygon->x[j],
                            polygon->y[j]);
          else
            printf("      ( %g %g )\n", polygon->x[j],
                            polygon->y[j]);
          }
      break;
        case lefiGeomPolygonIterE:
            polygonIter = geometry->getPolygonIter(i);
            if (polygonIter->colorMask != 0) {
                printf("       POLYGON MASK %d ITERATE ", polygonIter->colorMask);
            } else {
                printf("      POLYGON ITERATE");
            }
            for (j = 0; j < polygonIter->numPoints; j++)
                printf("      ( %g %g )\n", polygonIter->x[j],
                        polygonIter->y[j]);
            printf("      DO %g BY %g STEP %g %g ;\n",
                    polygonIter->xStart, polygonIter->yStart,
                    polygonIter->xStep, polygonIter->yStep);
            break;
        case lefiGeomViaE:
            via = geometry->getVia(i);
            if (via->topMaskNum != 0 || via->bottomMaskNum != 0 || via->cutMaskNum !=0) {
                printf("      VIA MASK %d%d%d ( %g %g ) %s ;\n",
                        via->topMaskNum, via->cutMaskNum, via->bottomMaskNum,
                        via->x, via->y,
                        via->name);

            } else {
                printf("      VIA ( %g %g ) %s ;\n", via->x, via->y,
                        via->name);
            }
            break;
        case lefiGeomViaIterE:
            viaIter = geometry->getViaIter(i);
            if (viaIter->topMaskNum != 0 || viaIter->cutMaskNum != 0 || viaIter->bottomMaskNum != 0) {
                printf("      VIA ITERATE MASK %d%d%d ( %g %g ) %s\n",
                        viaIter->topMaskNum, viaIter->cutMaskNum, viaIter->bottomMaskNum,
                        viaIter->x,
                        viaIter->y, viaIter->name);
            } else {
                printf("      VIA ITERATE ( %g %g ) %s\n", viaIter->x,
                        viaIter->y, viaIter->name);
            }
            printf("      DO %g BY %g STEP %g %g ;\n",
                    viaIter->xStart, viaIter->yStart,
                    viaIter->xStep, viaIter->yStep);
            break;
        default:
            printf("BOGUS geometries type.\n");
            break;
        }
    }
}

// Function: units_cbk
// [UNITS
// [TIME NANOSECONDS convertFactor ;]
// [CAPACITANCE PICOFARADS convertFactor ;]
// [RESISTANCE OHMS convertFactor ;]
// [POWER MILLIWATTS convertFactor ;]
// [CURRENT MILLIAMPS convertFactor ;]
// [VOLTAGE VOLTS convertFactor ;]
// [DATABASE MICRONS LEFconvertFactor ;]
// [FREQUENCY MEGAHERTZ convertFactor ;]
// END UNITS]
int LEFReader::units_cbk(lefrCallbackType_e c, lefiUnits* unit, void* ud) {

  CHECK(c == lefrUnitsCbkType);
  
  LEFReader* reader = (LEFReader*)ud;

  if (unit->hasDatabase()) {
    #ifdef LEF_CALLBACK_VERBOSITY
    printf("UNITS DATABASE %s %g\n", unit->databaseName(), reader->chkNum(unit->databaseNumber()));
    #endif
    reader->timer_ptr()->techlib_ptr()->set_database_unit(reader->chkNum(unit->databaseNumber()));
  }
  // BEG TODO--------------------------------------------------------------------------------------
  if (unit->hasCapacitance())
    printf("UNITS CAPACITANCE PICOFARADS %g\n", reader->chkNum(unit->capacitance()));
  if (unit->hasResistance())
    printf("UNITS RESISTANCE OHMS %g\n", reader->chkNum(unit->resistance()));
  if (unit->hasPower())
    printf("UNITS POWER MILLIWATTS %g\n", reader->chkNum(unit->power()));
  if (unit->hasCurrent())
    printf("UNITS CURRENT MILLIAMPS %g\n", reader->chkNum(unit->current()));
  if (unit->hasVoltage())
    printf("UNITS VOLTAGE VOLTS %g\n", reader->chkNum(unit->voltage()));
  if (unit->hasFrequency())
    printf("UNITS FREQUENCY MEGAHERTZ %g\n", reader->chkNum(unit->frequency()));
  // END TODO--------------------------------------------------------------------------------------
  return 0;
}

// Function: site_cbk
//
// SITE siteName
//
// CLASS {PAD | CORE} ;
//
// [SYMMETRY {X | Y | R90} ... ;] 
//
// SIZE width BY height ;
//
// END siteName
//
int LEFReader::site_cbk(lefrCallbackType_e c, lefiSite* site, void* ud) {

  CHECK(c == lefrSiteCbkType);

  // Retrieve the cursor.
  LEFReader* reader = (LEFReader*)ud;
  reader->_site_cursor = reader->timer_ptr()->techlib_ptr()->insert_site(site->name()); 

  int i, hasPrtSym = 0;
  #ifdef LEF_CALLBACK_VERBOSITY 
  printf("SITE %s\n", site->name());
  #endif
  if (site->hasClass()) {
     #ifdef LEF_CALLBACK_VERBOSITY 
     printf("  CLASS %s ;\n", site->siteClass());
     #endif
    if(strcmp(site->siteClass(), "PAD") == 0) {
      reader->site_cursor()->set_class(PAD_SITE_CLASS);
    }
    else if(strcmp(site->siteClass(), "CORE") == 0) {
      reader->site_cursor()->set_class(CORE_SITE_CLASS);
    }
    else {
      CHECK(false);
    }
  }
  if (site->hasXSymmetry()) {
     #ifdef LEF_CALLBACK_VERBOSITY 
     printf("  SYMMETRY X ");
     #endif
     reader->site_cursor()->set_is_x_symmetry(true);
     hasPrtSym = 1;
  }
  if (site->hasYSymmetry()) {
     if (hasPrtSym) {
       #ifdef LEF_CALLBACK_VERBOSITY 
       printf("Y ");
       #endif
     }
     else {
       #ifdef LEF_CALLBACK_VERBOSITY 
       printf("  SYMMETRY Y ");
       #endif
       hasPrtSym = 1;
     }
     reader->site_cursor()->set_is_y_symmetry(true);
  }
  if (site->has90Symmetry()) {
     if (hasPrtSym) {
       #ifdef LEF_CALLBACK_VERBOSITY 
       printf("R90 ");
       #endif
     }
     else {
       #ifdef LEF_CALLBACK_VERBOSITY 
       printf("  SYMMETRY R90 ");
       #endif
       hasPrtSym = 1;
     }
     reader->site_cursor()->set_is_r90_symmetry(true);
  }
  if (hasPrtSym) {
    #ifdef LEF_CALLBACK_VERBOSITY 
    printf(";\n");
    #endif
  }
  if (site->hasSize()) {
    #ifdef LEF_CALLBACK_VERBOSITY 
    printf("  SIZE %g BY %g ;\n", site->sizeX(), site->sizeY());
    #endif
    reader->site_cursor()->set_width(site->sizeX());
    reader->site_cursor()->set_height(site->sizeY());
  }

  if (site->hasRowPattern()) {
    #ifdef LEF_CALLBACK_VERBOSITY 
    printf("  ROWPATTERN ");
    #endif
     for (i = 0; i < site->numSites(); i++) {
       #ifdef LEF_CALLBACK_VERBOSITY 
       printf("  %s %s ", site->siteName(i), site->siteOrientStr(i));
       #endif
     }
     #ifdef LEF_CALLBACK_VERBOSITY 
     printf(";\n");
     #endif
  }


  #ifdef LEF_CALLBACK_VERBOSITY 
  printf("END %s\n", site->name());
  #endif
  return 0;
}

// Function: layer_cbk
// LAYER layerName
// TYPE CUT ;
// [MASK maskNum ;]
// [SPACING cutSpacing
//   [CENTERTOCENTER]
//   [SAMENET]
//   [ LAYER secondLayerName [STACK]
//   | ADJACENTCUTS {2 | 3 | 4} WITHIN cutWithin [EXCEPTSAMEPGNET]
//   | PARALLELOVERLAP
//   | AREA cutArea
//   ]
// ;] ...
// [SPACINGTABLE ORTHOGONAL
//     {WITHIN cutWithin SPACING orthoSpacing} ... ;]
// [ARRAYSPACING [LONGARRAY] [WIDTH viaWidth] CUTSPACING cutSpacing 
//     {ARRAYCUTS arrayCuts SPACING arraySpacing} ... ;]
// [WIDTH minWidth ;]
// [ENCLOSURE [ABOVE | BELOW] overhang1 overhang2 
//   [ WIDTH minWidth [EXCEPTEXTRACUT cutWithin] 
//   | LENGTH minLength] 
// ;] ...
// [PREFERENCLOSURE [ABOVE | BELOW] overhang1 overhang2 [WIDTH minWidth] ;] ...
// [RESISTANCE resistancePerCut ;]
// [PROPERTY propName propVal ;] ...
// [ACCURRENTDENSITY {PEAK | AVERAGE | RMS}
//   { value 
//     | FREQUENCY freq_1 freq_2 ... ;
//       [CUTAREA cutArea_1 cutArea_2 ... ;]
//       TABLEENTRIES
//         v_freq_1_cutArea_1 v_freq_1_cutArea_2 ...
//         v_freq_2_cutArea_1 v_freq_2_cutArea_2 ...
//    ...
//   } 
// ;]
// [DCCURRENTDENSITY AVERAGE
//   { value 
//     | CUTAREA cutArea_1 cutArea_2 ... ;
//     TABLEENTRIES value_1 value_2 ...
//   } 
// ;]
// [ANTENNAMODEL {OXIDE1 | OXIDE2 | OXIDE3 | OXIDE4} ;] ...
// [ANTENNAAREARATIO value ;] ...
// [ANTENNADIFFAREARATIO {value | PWL ( ( d1 r1 ) ( d2 r2 ) ...)} ;] ...
// [ANTENNACUMAREARATIO value ;] ...
// [ANTENNACUMDIFFAREARATIO {value | PWL ( ( d1 r1 ) ( d2 r2 ) ...)} ;] ...
// [ANTENNAAREAFACTOR value [DIFFUSEONLY] ;] ...
// [ANTENNACUMROUTINGPLUSCUT ;]
// [ANTENNAGATEPLUSDIFF plusDiffFactor ;]
// [ANTENNAAREAMINUSDIFF minusDiffFactor ;]
// [ANTENNAAREADIFFREDUCEPWL 
// ( ( diffArea1 diffAreaFactor1 ) ( diffArea2 diffAreaFactor2 ) ...) ; ]
int LEFReader::layer_cbk(lefrCallbackType_e c, lefiLayer* layer, void* ud) {
  
  CHECK(c == lefrLayerCbkType);
  
  LEFReader* reader = (LEFReader*)ud;
  reader->_layer_cursor = reader->timer_ptr()->techlib_ptr()->insert_layer(layer->name()); 

  int i, j, k;
  int numPoints, propNum;
  double *widths, *current;
  lefiLayerDensity* density;
  lefiAntennaPWL* pwl;
  lefiSpacingTable* spTable;
  lefiInfluence* influence;
  lefiParallel* parallel;
  lefiTwoWidths* twoWidths;
  char pType;
  int numMinCut, numMinenclosed;
  lefiAntennaModel* aModel;
  lefiOrthogonal*   ortho;

  // Call parse65nmRules for 5.7 syntax in 5.6
  if (reader->parse65nm()) layer->parse65nmRules();

  // Call parseLef58Type for 5.8 syntax in 5.7
  if (reader->parseLef58Type()) layer->parseLEF58Layer();

  #ifdef LEF_CALLBACK_VERBOSITY
  printf("LAYER %s\n", layer->name());
  #endif

  if (layer->hasType()) {
    #ifdef LEF_CALLBACK_VERBOSITY
    printf("  TYPE %s ;\n", layer->type());
    #endif
    if(strcmp(layer->type(), "ROUTINE")) {
      reader->layer_cursor()->set_type(ROUTING_LAYER_TYPE);
    }
    else if(strcmp(layer->type(), "CUT")) {
      reader->layer_cursor()->set_type(CUT_LAYER_TYPE);
    }
    else if(strcmp(layer->type(), "OVERLAP")) {
      reader->layer_cursor()->set_type(OVERLAP_LAYER_TYPE);
    }
    else if(strcmp(layer->type(), "MASTERSLICE")) {
      reader->layer_cursor()->set_type(MASTERSLICE_LAYER_TYPE);
    }
    else {
      CHECK(false);
    }
  }
  if (layer->hasLayerType()) {
    printf("  LAYER TYPE %s ;\n", layer->layerType());
  }
  if (layer->hasMask()) {
    printf("  MASK %d ;\n", layer->mask());
  }

  if (layer->hasPitch()) {
    #ifdef LEF_CALLBACK_VERBOSITY
    printf("  PITCH %g ;\n", layer->pitch());
    #endif
    reader->layer_cursor()->set_xpitch(layer->pitch());
    reader->layer_cursor()->set_ypitch(layer->pitch());
  }
  else if (layer->hasXYPitch()) {
    #ifdef LEF_CALLBACK_VERBOSITY
    printf("  PITCH %g %g ;\n", layer->pitchX(), layer->pitchY());
    #endif
    reader->layer_cursor()->set_xpitch(layer->pitchX());
    reader->layer_cursor()->set_ypitch(layer->pitchY());
  }

  if (layer->hasOffset()) {
    #ifdef LEF_CALLBACK_VERBOSITY
    printf("  OFFSET %g ;\n", layer->offset());
    #endif
    reader->layer_cursor()->set_xoffset(layer->offset());
    reader->layer_cursor()->set_yoffset(layer->offset());
  }
  else if (layer->hasXYOffset()) {
    #ifdef LEF_CALLBACK_VERBOSITY
    printf("  OFFSET %g %g ;\n", layer->offsetX(), layer->offsetY());
    #endif
    reader->layer_cursor()->set_xoffset(layer->offsetX());
    reader->layer_cursor()->set_yoffset(layer->offsetY());
  }

  // BEG TODO--------------------------------------------------------------------------------------
  if (layer->hasDiagPitch()) {
    #ifdef LEF_CALLBACK_VERBOSITY
    printf("  DIAGPITCH %g ;\n", layer->diagPitch());
    #endif
  }
  else if (layer->hasXYDiagPitch()) {
    #ifdef LEF_CALLBACK_VERBOSITY
    printf("  DIAGPITCH %g %g ;\n", layer->diagPitchX(), layer->diagPitchY());
    #endif
  }
  if (layer->hasDiagWidth()) {
    #ifdef LEF_CALLBACK_VERBOSITY
    printf("  DIAGWIDTH %g ;\n", layer->diagWidth());
    #endif
  }
  if (layer->hasDiagSpacing()) {
    #ifdef LEF_CALLBACK_VERBOSITY
    printf("  DIAGSPACING %g ;\n", layer->diagSpacing());
    #endif
  }
  // END TODO--------------------------------------------------------------------------------------

  if (layer->hasWidth()) {
    #ifdef LEF_CALLBACK_VERBOSITY
    printf("  WIDTH %g ;\n", layer->width());
    #endif
    reader->layer_cursor()->set_width(layer->width());
  }

  // BEG TODO--------------------------------------------------------------------------------------
  if (layer->hasArea()) {
    printf("  AREA %g ;\n", layer->area());
  }
  if (layer->hasSlotWireWidth()) {
    printf("  SLOTWIREWIDTH %g ;\n", layer->slotWireWidth());
  }
  if (layer->hasSlotWireLength()) {
    printf("  SLOTWIRELENGTH %g ;\n", layer->slotWireLength());
  }
  if (layer->hasSlotWidth()) {
    printf("  SLOTWIDTH %g ;\n", layer->slotWidth());
  }
  if (layer->hasSlotLength()) {
    printf("  SLOTLENGTH %g ;\n", layer->slotLength());
  }
  if (layer->hasMaxAdjacentSlotSpacing()) {
    printf("  MAXADJACENTSLOTSPACING %g ;\n", layer->maxAdjacentSlotSpacing());
  }
  if (layer->hasMaxCoaxialSlotSpacing()) {
    printf("  MAXCOAXIALSLOTSPACING %g ;\n", layer->maxCoaxialSlotSpacing());
  }
  if (layer->hasMaxEdgeSlotSpacing()) {
    printf("  MAXEDGESLOTSPACING %g ;\n", layer->maxEdgeSlotSpacing());
  }
  if (layer->hasMaxFloatingArea()){          // 5.7
    printf("  MAXFLOATINGAREA %g ;\n", layer->maxFloatingArea());
  }
  if (layer->hasArraySpacing()) {           // 5.7
    printf("  ARRAYSPACING ");
    if (layer->hasLongArray()) {
      printf("LONGARRAY ");
    }
    if (layer->hasViaWidth()) {
      printf("WIDTH %g ", layer->viaWidth());
    }
    printf("CUTSPACING %g", layer->cutSpacing());
    for (i = 0; i < layer->numArrayCuts(); i++) {
      printf("\n\tARRAYCUTS %d SPACING %g", layer->arrayCuts(i), layer->arraySpacing(i));
    }
    #ifdef LEF_CALLBACK_VERBOSITY
    printf(" ;\n");
    #endif
  }
  if (layer->hasSplitWireWidth())
     printf("  SPLITWIREWIDTH %g ;\n", layer->splitWireWidth());
  if (layer->hasMinimumDensity())
     printf("  MINIMUMDENSITY %g ;\n", layer->minimumDensity());
  if (layer->hasMaximumDensity())
     printf("  MAXIMUMDENSITY %g ;\n", layer->maximumDensity());
  if (layer->hasDensityCheckWindow())
     printf("  DENSITYCHECKWINDOW %g %g ;\n", layer->densityCheckWindowLength(), layer->densityCheckWindowWidth());
  if (layer->hasDensityCheckStep())
     printf("  DENSITYCHECKSTEP %g ;\n", layer->densityCheckStep());
  if (layer->hasFillActiveSpacing())
     printf("  FILLACTIVESPACING %g ;\n",layer->fillActiveSpacing());
  // 5.4.1
  numMinCut = layer->numMinimumcut();
  if (numMinCut > 0) {
     for (i = 0; i < numMinCut; i++) {
         printf("  MINIMUMCUT %d WIDTH %g ",
              layer->minimumcut(i),
              layer->minimumcutWidth(i));
         if (layer->hasMinimumcutWithin(i))
            printf("WITHIN %g ", layer->minimumcutWithin(i));
         if (layer->hasMinimumcutConnection(i))
            printf("%s ", layer->minimumcutConnection(i));
         if (layer->hasMinimumcutNumCuts(i))
            printf("LENGTH %g WITHIN %g ",
            layer->minimumcutLength(i),
            layer->minimumcutDistance(i));
         printf(";\n");
     }
  }
  // 5.4.1
  if (layer->hasMaxwidth()) {
     printf("  MAXWIDTH %g ;\n", layer->maxwidth());
  }
  // 5.5
  if (layer->hasMinwidth()) {
     printf("  MINWIDTH %g ;\n", layer->minwidth());
  }
  // 5.5
  numMinenclosed = layer->numMinenclosedarea();
  if (numMinenclosed > 0) {
     for (i = 0; i < numMinenclosed; i++) {
         printf("  MINENCLOSEDAREA %g ",
              layer->minenclosedarea(i));
         if (layer->hasMinenclosedareaWidth(i))
              printf("MINENCLOSEDAREAWIDTH %g ",
                      layer->minenclosedareaWidth(i));
         printf (";\n"); 
     }
  }
  // 5.4.1 & 5.6
  if (layer->hasMinstep()) {
     for (i = 0; i < layer->numMinstep(); i++) {
        printf("  MINSTEP %g ", layer->minstep(i));
        if (layer->hasMinstepType(i))
           printf("%s ", layer->minstepType(i));
        if (layer->hasMinstepLengthsum(i))
           printf("LENGTHSUM %g ",
                   layer->minstepLengthsum(i));
        if (layer->hasMinstepMaxedges(i))
           printf("MAXEDGES %d ", layer->minstepMaxedges(i));
        if (layer->hasMinstepMinAdjLength(i))
           printf("MINADJLENGTH %g ", layer->minstepMinAdjLength(i));
        if (layer->hasMinstepMinBetLength(i))
           printf("MINBETLENGTH %g ", layer->minstepMinBetLength(i));
        if (layer->hasMinstepXSameCorners(i))
           printf("XSAMECORNERS");
        printf(";\n");
     }
  }
  // 5.4.1
  if (layer->hasProtrusion()) {
     printf("  PROTRUSIONWIDTH %g LENGTH %g WIDTH %g ;\n",
             layer->protrusionWidth1(),
             layer->protrusionLength(),
             layer->protrusionWidth2());
  } 
  if (layer->hasSpacingNumber()) {
     for (i = 0; i < layer->numSpacing(); i++) {
       printf("  SPACING %g ", layer->spacing(i));
       if (layer->hasSpacingName(i))
          printf("LAYER %s ", layer->spacingName(i));
       if (layer->hasSpacingLayerStack(i))
          printf("STACK ");                           // 5.7
       if (layer->hasSpacingAdjacent(i))
          printf("ADJACENTCUTS %d WITHIN %g ",
                  layer->spacingAdjacentCuts(i),
                  layer->spacingAdjacentWithin(i));
       if (layer->hasSpacingAdjacentExcept(i))    // 5.7
          printf("EXCEPTSAMEPGNET "); 
       if (layer->hasSpacingCenterToCenter(i))
          printf("CENTERTOCENTER ");
       if (layer->hasSpacingSamenet(i))           // 5.7
          printf("SAMENET ");
           if (layer->hasSpacingSamenetPGonly(i)) // 5.7
              printf("PGONLY ");
       if (layer->hasSpacingArea(i))              // 5.7
          printf("AREA %g ", layer->spacingArea(i));
       if (layer->hasSpacingRange(i)) {
          printf("RANGE %g %g ", layer->spacingRangeMin(i),
                  layer->spacingRangeMax(i));
          if (layer->hasSpacingRangeUseLengthThreshold(i))
             printf("USELENGTHTHRESHOLD "); 
          else if (layer->hasSpacingRangeInfluence(i)) {
              printf("INFLUENCE %g ",
                 layer->spacingRangeInfluence(i));
              if (layer->hasSpacingRangeInfluenceRange(i))
                 printf("RANGE %g %g ",
                    layer->spacingRangeInfluenceMin(i),
                    layer->spacingRangeInfluenceMax(i));
           } else if (layer->hasSpacingRangeRange(i))
               printf("RANGE %g %g ",
                 layer->spacingRangeRangeMin(i),
                 layer->spacingRangeRangeMax(i));
       } else if (layer->hasSpacingLengthThreshold(i)) {
           printf("LENGTHTHRESHOLD %g ",
              layer->spacingLengthThreshold(i));
           if (layer->hasSpacingLengthThresholdRange(i))
              printf("RANGE %g %g",
                 layer->spacingLengthThresholdRangeMin(i),
                 layer->spacingLengthThresholdRangeMax(i));
       } else if (layer->hasSpacingNotchLength(i)) {// 5.7
           printf("NOTCHLENGTH %g",
                   layer->spacingNotchLength(i));
       } else if (layer->hasSpacingEndOfNotchWidth(i)) // 5.7
           printf("ENDOFNOTCHWIDTH %g NOTCHSPACING %g, NOTCHLENGTH %g",
                   layer->spacingEndOfNotchWidth(i),
                   layer->spacingEndOfNotchSpacing(i),
                   layer->spacingEndOfNotchLength(i));

       if (layer->hasSpacingParallelOverlap(i))   // 5.7
          printf("PARALLELOVERLAP "); 
       if (layer->hasSpacingEndOfLine(i)) {       // 5.7
          printf("ENDOFLINE %g WITHIN %g ",
             layer->spacingEolWidth(i),
             layer->spacingEolWithin(i));
          if (layer->hasSpacingParellelEdge(i)) {
             printf("PARALLELEDGE %g WITHIN %g ",
                layer->spacingParSpace(i),
                layer->spacingParWithin(i));
             if (layer->hasSpacingTwoEdges(i)) {
                printf("TWOEDGES ");
             }
          }
       }
       printf(";\n");
     }
  }
  if (layer->hasSpacingTableOrtho()) {            // 5.7
     printf("SPACINGTABLE ORTHOGONAL"); 
     ortho = layer->orthogonal();
     for (i = 0; i < ortho->lefiOrthogonal::numOrthogonal(); i++) {
        printf("\n   WITHIN %g SPACING %g",
                ortho->lefiOrthogonal::cutWithin(i),
                ortho->lefiOrthogonal::orthoSpacing(i));
     }
     printf(";\n");
  }
  for (i = 0; i < layer->numEnclosure(); i++) {
     printf("ENCLOSURE ");
     if (layer->hasEnclosureRule(i))
        printf("%s ", layer->enclosureRule(i));
     printf("%g %g ", layer->enclosureOverhang1(i),
                             layer->enclosureOverhang2(i));
     if (layer->hasEnclosureWidth(i))
        printf("WIDTH %g ", layer->enclosureMinWidth(i));
     if (layer->hasEnclosureExceptExtraCut(i))
        printf("EXCEPTEXTRACUT %g ",
                layer->enclosureExceptExtraCut(i));
     if (layer->hasEnclosureMinLength(i))
        printf("LENGTH %g ", layer->enclosureMinLength(i));
     printf(";\n");
  }
  for (i = 0; i < layer->numPreferEnclosure(); i++) {
     printf("PREFERENCLOSURE ");
     if (layer->hasPreferEnclosureRule(i))
        printf("%s ", layer->preferEnclosureRule(i));
     printf("%g %g ", layer->preferEnclosureOverhang1(i),
                             layer->preferEnclosureOverhang2(i));
     if (layer->hasPreferEnclosureWidth(i))
        printf("WIDTH %g ",layer->preferEnclosureMinWidth(i));
     printf(";\n");
  }
  if (layer->hasResistancePerCut())
     printf("  RESISTANCE %g ;\n",
             layer->resistancePerCut());
  if (layer->hasCurrentDensityPoint())
     printf("  CURRENTDEN %g ;\n",
             layer->currentDensityPoint());
  if (layer->hasCurrentDensityArray()) { 
     layer->currentDensityArray(&numPoints, &widths, &current);
     for (i = 0; i < numPoints; i++)
         printf("  CURRENTDEN ( %g %g ) ;\n", widths[i], current[i]);
  }
  if (layer->hasDirection()) {
    #ifdef LEF_CALLBACK_VERBOSITY
    printf("  DIRECTION %s ;\n", layer->direction());
    #endif 
    if(strcmp(layer->direction(), "HORIZONTAL") == 0) {
      reader->layer_cursor()->set_direction(HORIZONTAL_LAYER_DIRECTION);
    }
    else if(strcmp(layer->direction(), "VERTICAL") == 0) {
      reader->layer_cursor()->set_direction(VERTICAL_LAYER_DIRECTION);
    }
    else {
      CHECK(false);
    }
  }
  if (layer->hasResistance())
     printf("  RESISTANCE RPERSQ %g ;\n",
             layer->resistance());
  if (layer->hasCapacitance())
     printf("  CAPACITANCE CPERSQDIST %g ;\n",
             layer->capacitance());
  if (layer->hasEdgeCap())
     printf("  EDGECAPACITANCE %g ;\n", layer->edgeCap());
  if (layer->hasHeight())
     printf("  TYPE %g ;\n", layer->height());
  if (layer->hasThickness())
     printf("  THICKNESS %g ;\n", layer->thickness());
  if (layer->hasWireExtension())
     printf("  WIREEXTENSION %g ;\n", layer->wireExtension());
  if (layer->hasShrinkage())
     printf("  SHRINKAGE %g ;\n", layer->shrinkage());
  if (layer->hasCapMultiplier())
     printf("  CAPMULTIPLIER %g ;\n", layer->capMultiplier());
  if (layer->hasAntennaArea())
     printf("  ANTENNAAREAFACTOR %g ;\n",
             layer->antennaArea());
  if (layer->hasAntennaLength())
     printf("  ANTENNALENGTHFACTOR %g ;\n",
             layer->antennaLength());

  // 5.5 AntennaModel
  for (i = 0; i < layer->numAntennaModel(); i++) {
     aModel = layer->antennaModel(i);

     printf("  ANTENNAMODEL %s ;\n",
             aModel->lefiAntennaModel::antennaOxide());
     
     if (aModel->lefiAntennaModel::hasAntennaAreaRatio())
        printf("  ANTENNAAREARATIO %g ;\n",
                aModel->lefiAntennaModel::antennaAreaRatio());
     if (aModel->lefiAntennaModel::hasAntennaDiffAreaRatio())
        printf("  ANTENNADIFFAREARATIO %g ;\n",
                aModel->lefiAntennaModel::antennaDiffAreaRatio());
     else if (aModel->lefiAntennaModel::hasAntennaDiffAreaRatioPWL()) {
        pwl = aModel->lefiAntennaModel::antennaDiffAreaRatioPWL();
        printf("  ANTENNADIFFAREARATIO PWL ( ");
        for (j = 0; j < pwl->lefiAntennaPWL::numPWL(); j++)
           printf("( %g %g ) ", pwl->lefiAntennaPWL::PWLdiffusion(j),
                   pwl->lefiAntennaPWL::PWLratio(j));
        printf(") ;\n");
     }
     if (aModel->lefiAntennaModel::hasAntennaCumAreaRatio())
        printf("  ANTENNACUMAREARATIO %g ;\n",
                aModel->lefiAntennaModel::antennaCumAreaRatio());
     if (aModel->lefiAntennaModel::hasAntennaCumDiffAreaRatio())
        printf("  ANTENNACUMDIFFAREARATIO %g\n",
                aModel->lefiAntennaModel::antennaCumDiffAreaRatio());
     if (aModel->lefiAntennaModel::hasAntennaCumDiffAreaRatioPWL()) {
        pwl = aModel->lefiAntennaModel::antennaCumDiffAreaRatioPWL();
        printf("  ANTENNACUMDIFFAREARATIO PWL ( ");
        for (j = 0; j < pwl->lefiAntennaPWL::numPWL(); j++)
           printf("( %g %g ) ", pwl->lefiAntennaPWL::PWLdiffusion(j),
                   pwl->lefiAntennaPWL::PWLratio(j));
        printf(") ;\n");
     }
     if (aModel->lefiAntennaModel::hasAntennaAreaFactor()) {
        printf("  ANTENNAAREAFACTOR %g ",
                aModel->lefiAntennaModel::antennaAreaFactor());
        if (aModel->lefiAntennaModel::hasAntennaAreaFactorDUO())
           printf("  DIFFUSEONLY ");
        printf(";\n");
     }
     if (aModel->lefiAntennaModel::hasAntennaSideAreaRatio())
        printf("  ANTENNASIDEAREARATIO %g ;\n",
                aModel->lefiAntennaModel::antennaSideAreaRatio());
     if (aModel->lefiAntennaModel::hasAntennaDiffSideAreaRatio())
        printf("  ANTENNADIFFSIDEAREARATIO %g\n",
                aModel->lefiAntennaModel::antennaDiffSideAreaRatio());
     else if (aModel->lefiAntennaModel::hasAntennaDiffSideAreaRatioPWL()) {
        pwl = aModel->lefiAntennaModel::antennaDiffSideAreaRatioPWL();
        printf("  ANTENNADIFFSIDEAREARATIO PWL ( ");
        for (j = 0; j < pwl->lefiAntennaPWL::numPWL(); j++)
           printf("( %g %g ) ", pwl->lefiAntennaPWL::PWLdiffusion(j),
                   pwl->lefiAntennaPWL::PWLratio(j));
        printf(") ;\n");
     }
     if (aModel->lefiAntennaModel::hasAntennaCumSideAreaRatio())
        printf("  ANTENNACUMSIDEAREARATIO %g ;\n",
                aModel->lefiAntennaModel::antennaCumSideAreaRatio());
     if (aModel->lefiAntennaModel::hasAntennaCumDiffSideAreaRatio())
        printf("  ANTENNACUMDIFFSIDEAREARATIO %g\n",
                aModel->lefiAntennaModel::antennaCumDiffSideAreaRatio());
     else if (aModel->lefiAntennaModel::hasAntennaCumDiffSideAreaRatioPWL()) {
        pwl = aModel->lefiAntennaModel::antennaCumDiffSideAreaRatioPWL();
        printf("  ANTENNACUMDIFFSIDEAREARATIO PWL ( ");
        for (j = 0; j < pwl->lefiAntennaPWL::numPWL(); j++)
           printf("( %g %g ) ", pwl->lefiAntennaPWL::PWLdiffusion(j),
                   pwl->lefiAntennaPWL::PWLratio(j));
        printf(") ;\n");
     }
     if (aModel->lefiAntennaModel::hasAntennaSideAreaFactor()) {
        printf("  ANTENNASIDEAREAFACTOR %g ",
                aModel->lefiAntennaModel::antennaSideAreaFactor());
        if (aModel->lefiAntennaModel::hasAntennaSideAreaFactorDUO())
           printf("  DIFFUSEONLY ");
        printf(";\n");
     }
     if (aModel->lefiAntennaModel::hasAntennaCumRoutingPlusCut())
        printf("  ANTENNACUMROUTINGPLUSCUT ;\n");
     if (aModel->lefiAntennaModel::hasAntennaGatePlusDiff())
        printf("  ANTENNAGATEPLUSDIFF %g ;\n",
                aModel->lefiAntennaModel::antennaGatePlusDiff());
     if (aModel->lefiAntennaModel::hasAntennaAreaMinusDiff())
        printf("  ANTENNAAREAMINUSDIFF %g ;\n",
                aModel->lefiAntennaModel::antennaAreaMinusDiff());
     if (aModel->lefiAntennaModel::hasAntennaAreaDiffReducePWL()) {
        pwl = aModel->lefiAntennaModel::antennaAreaDiffReducePWL();
        printf("  ANTENNAAREADIFFREDUCEPWL ( ");
        for (j = 0; j < pwl->lefiAntennaPWL::numPWL(); j++)
           printf("( %g %g ) ", pwl->lefiAntennaPWL::PWLdiffusion(j),
                   pwl->lefiAntennaPWL::PWLratio(j));
        printf(") ;\n");
     }
  }

  if (layer->numAccurrentDensity()) {
     for (i = 0; i < layer->numAccurrentDensity(); i++) {
         density = layer->accurrent(i);
         printf("  ACCURRENTDENSITY %s", density->type()); 
         if (density->hasOneEntry())
             printf(" %g ;\n", density->oneEntry()); 
         else {
             printf("\n");
             if (density->numFrequency()) {
                printf("    FREQUENCY");
                for (j = 0; j < density->numFrequency(); j++)
                   printf(" %g", density->frequency(j));
                printf(" ;\n");    
             }
             if (density->numCutareas()) {
                printf("    CUTAREA");
                for (j = 0; j < density->numCutareas(); j++)
                   printf(" %g", density->cutArea(j));
                printf(" ;\n");    
             }
             if (density->numWidths()) {
                printf("    WIDTH");
                for (j = 0; j < density->numWidths(); j++)
                   printf(" %g", density->width(j));
                printf(" ;\n");    
             }
             if (density->numTableEntries()) {
                k = 5;
                printf("    TABLEENTRIES");
                for (j = 0; j < density->numTableEntries(); j++)
                   if (k > 4) {
                      printf("\n     %g", density->tableEntry(j));
                      k = 1;
                   } else {
                      printf(" %g", density->tableEntry(j));
                      k++;
                   }
                printf(" ;\n");    
             }
         }
     }
  }
  if (layer->numDccurrentDensity()) {
     for (i = 0; i < layer->numDccurrentDensity(); i++) {
         density = layer->dccurrent(i);
         printf("  DCCURRENTDENSITY %s", density->type()); 
         if (density->hasOneEntry())
             printf(" %g ;\n", density->oneEntry()); 
         else {
             printf("\n");
             if (density->numCutareas()) {
                printf("    CUTAREA");
                for (j = 0; j < density->numCutareas(); j++)
                   printf(" %g", density->cutArea(j));
                printf(" ;\n");    
             }
             if (density->numWidths()) {
                printf("    WIDTH");
                for (j = 0; j < density->numWidths(); j++)
                   printf(" %g", density->width(j));
                printf(" ;\n");    
             }
             if (density->numTableEntries()) {
                printf("    TABLEENTRIES");
                for (j = 0; j < density->numTableEntries(); j++)
                   printf(" %g", density->tableEntry(j));
                printf(" ;\n");    
             }
         }
     }
  }

  for (i = 0; i < layer->numSpacingTable(); i++) {
     spTable = layer->spacingTable(i);
     printf("   SPACINGTABLE\n");
     if (spTable->lefiSpacingTable::isInfluence()) {
        influence = spTable->lefiSpacingTable::influence();
        printf("      INFLUENCE");
        for (j = 0; j < influence->lefiInfluence::numInfluenceEntry(); j++) {
           printf("\n          WIDTH %g WITHIN %g SPACING %g",
                   influence->lefiInfluence::width(j),
                   influence->lefiInfluence::distance(j),
                   influence->lefiInfluence::spacing(j));
        }   
        printf(" ;\n");
     } else if (spTable->lefiSpacingTable::isParallel()){
        parallel = spTable->lefiSpacingTable::parallel();
        printf("      PARALLELRUNLENGTH");
        for (j = 0; j < parallel->lefiParallel::numLength(); j++) {
           printf(" %g", parallel->lefiParallel::length(j));
        }
        for (j = 0; j < parallel->lefiParallel::numWidth(); j++) {
           printf("\n          WIDTH %g",
                   parallel->lefiParallel::width(j));
           for (k = 0; k < parallel->lefiParallel::numLength(); k++) {
              printf(" %g", parallel->lefiParallel::widthSpacing(j, k));
           }
        }
        printf(" ;\n");
     } else {    // 5.7 TWOWIDTHS
        twoWidths = spTable->lefiSpacingTable::twoWidths();
        printf("      TWOWIDTHS"); 
        for (j = 0; j < twoWidths->lefiTwoWidths::numWidth(); j++) {
           printf("\n          WIDTH %g ",
                   twoWidths->lefiTwoWidths::width(j));
           if (twoWidths->lefiTwoWidths::hasWidthPRL(j))
              printf("PRL %g ", twoWidths->lefiTwoWidths::widthPRL(j));
           for (k = 0; k < twoWidths->lefiTwoWidths::numWidthSpacing(j); k++)
              printf("%g ",twoWidths->lefiTwoWidths::widthSpacing(j, k));
        }
        printf(" ;\n");
     }
  }

  propNum = layer->numProps();
  if (propNum > 0) {
    printf("  PROPERTY ");
    for (i = 0; i < propNum; i++) {
       // value can either be a string or number
       printf("%s ", layer->propName(i));
       if (layer->propIsNumber(i))
            printf("%g ", layer->propNumber(i));
        if (layer->propIsString(i)) 
            printf("%s ", layer->propValue(i));
        pType = layer->propType(i);
        switch (pType) {
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
    printf(";\n");
  }
  if (layer->hasDiagMinEdgeLength()) {
    printf("  DIAGMINEDGELENGTH %g ;\n", layer->diagMinEdgeLength());
  }
  if (layer->numMinSize()) {
    printf("  MINSIZE ");
    for (i = 0; i < layer->numMinSize(); i++) {
      printf("%g %g ", layer->minSizeWidth(i), layer->minSizeLength(i)); 
    }
    printf(";\n");
  }
  // END TODO--------------------------------------------------------------------------------------

  #ifdef LEF_CALLBACK_VERBOSITY
  printf("END %s\n", layer->name()); 
  #endif

  // Set it to case sensitive from here on
  //lefrSetCaseSensitivity(1);

  return 0;
}

// Function: macro_begin_cbk
// MACRO macroName
//
// [CLASS
//
// { COVER [BUMP]
//
// | RING
//
// | BLOCK [BLACKBOX]
//
// | PAD [INPUT | OUTPUT |INOUT | POWER | SPACER | AREAIO]
//
// | CORE [FEEDTHRU | TIEHIGH | TIELOW | SPACER | ANTENNACELL]
//
// | ENDCAP {PRE | POST | TOPLEFT | TOPRIGHT | BOTTOMLEFT | BOTTOMRIGHT}
//
// }
//
// ;]
//
// [SOURCE {USER | BLOCK} ;]
//
// [FOREIGN foreignCellName [pt [orient]] ;] ...
//
// [ORIGIN pt ;]
//
// [SIZE width BY height ;]
//
// [SYMMETRY {X | Y | R90} ... ;]
//
// [SITE siteName ;]
//
// [PIN statement] ...
//
// [OBS statement] ...
//
int LEFReader::macro_begin_cbk(lefrCallbackType_e c, const char* macroName, void* ud) {

  CHECK(c == lefrMacroBeginCbkType);

  #ifdef LEF_CALLBACK_VERBOSITY
  printf("MACRO %s\n",  macroName);
  #endif

  // Retrieve the cursor.
  LEFReader* reader = (LEFReader*)ud;
  reader->_macro_cursor = reader->timer_ptr()->techlib_ptr()->insert_macro(macroName); 

  return 0;
}

// Function: macro_cbk
// MACRO macroName
//
// [CLASS
//
// { COVER [BUMP]
//
// | RING
//
// | BLOCK [BLACKBOX]
//
// | PAD [INPUT | OUTPUT |INOUT | POWER | SPACER | AREAIO]
//
// | CORE [FEEDTHRU | TIEHIGH | TIELOW | SPACER | ANTENNACELL]
//
// | ENDCAP {PRE | POST | TOPLEFT | TOPRIGHT | BOTTOMLEFT | BOTTOMRIGHT}
//
// }
//
// ;]
//
// [SOURCE {USER | BLOCK} ;]
//
// [FOREIGN foreignCellName [pt [orient]] ;] ...
//
// [ORIGIN pt ;]
//
// [SIZE width BY height ;]
//
// [SYMMETRY {X | Y | R90} ... ;]
//
// [SITE siteName ;]
//
// [PIN statement] ...
//
// [OBS statement] ...
//
int LEFReader::macro_cbk(lefrCallbackType_e c, lefiMacro* macro, void* ud) {
  
  CHECK(c == lefrMacroCbkType);
  
  LEFReader* reader = (LEFReader*)ud;
  
  CHECK(reader->macro_cursor() != nullptr);

  lefiSitePattern* pattern;
  int propNum, i, hasPrtSym = 0;

  if (macro->hasClass()) {
    #ifdef LEF_CALLBACK_VERBOSITY
    printf("  CLASS %s ;\n", macro->macroClass());
    #endif
    if(strcmp(macro->macroClass(), "PAD") == 0) {
      reader->macro_cursor()->set_class(PAD_MACRO_CLASS);
    }
    else if(strcmp(macro->macroClass(), "CORE") == 0) {
      reader->macro_cursor()->set_class(CORE_MACRO_CLASS);
    }
    else if(strcmp(macro->macroClass(), "BLOCK") == 0) {
      reader->macro_cursor()->set_class(BLOCK_MACRO_CLASS);
    }
    else {
      CHECK(false);
    }
  }
  if (macro->isFixedMask()) {
    #ifdef LEF_CALLBACK_VERBOSITY
    printf("  FIXEDMASK ;\n");
    #endif
    reader->macro_cursor()->set_is_fixed_mask(true);
  }
  if (macro->hasEEQ()) {
    printf("  EEQ %s ;\n", macro->EEQ());
  }
  if (macro->hasLEQ()) {
    printf("  LEQ %s ;\n", macro->LEQ());
  }
  if (macro->hasSource()) {
    printf("  SOURCE %s ;\n", macro->source());
  }
  if (macro->hasXSymmetry()) {
    #ifdef LEF_CALLBACK_VERBOSITY
    printf("  SYMMETRY X ");
    #endif
    hasPrtSym = 1;
    reader->macro_cursor()->set_is_x_symmetry(true);
  }
  if (macro->hasYSymmetry()) {   // print X Y & R90 in one line
    if (!hasPrtSym) {
      #ifdef LEF_CALLBACK_VERBOSITY
      printf("  SYMMETRY Y ");
      #endif
      hasPrtSym = 1;
    }
    else {
      #ifdef LEF_CALLBACK_VERBOSITY
      printf("Y ");
      #endif
    }
    reader->macro_cursor()->set_is_y_symmetry(true);
  }
  if (macro->has90Symmetry()) {
     if (!hasPrtSym) {
       #ifdef LEF_CALLBACK_VERBOSITY
       printf("  SYMMETRY R90 ");
       #endif
       hasPrtSym = 1;
     }
     else {
       #ifdef LEF_CALLBACK_VERBOSITY
       printf("R90 ");
       #endif
     }
    reader->macro_cursor()->set_is_r90_symmetry(true);
  }
  if (hasPrtSym) {
    #ifdef LEF_CALLBACK_VERBOSITY
    printf (";\n");
    #endif
    hasPrtSym = 0;
  }
  if (macro->hasSiteName()) {
    #ifdef LEF_CALLBACK_VERBOSITY
    printf("  SITE %s ;\n", macro->siteName());
    #endif
    reader->macro_cursor()->set_site_ptr(reader->timer_ptr()->techlib_ptr()->site_ptr(macro->siteName()));
  }
  if (macro->hasSitePattern()) {
     for (i = 0; i < macro->numSitePattern(); i++ ) {
       pattern = macro->sitePattern(i);
       if (pattern->lefiSitePattern::hasStepPattern()) {
          printf("  SITE %s %g %g %s DO %g BY %g STEP %g %g ;\n",
                pattern->lefiSitePattern::name(), pattern->lefiSitePattern::x(),
                pattern->lefiSitePattern::y(),
                reader->orientStr(pattern->lefiSitePattern::orient()),
                pattern->lefiSitePattern::xStart(),
                pattern->lefiSitePattern::yStart(),
                pattern->lefiSitePattern::xStep(),
                pattern->lefiSitePattern::yStep());
       } else {
          printf("  SITE %s %g %g %s ;\n",
                pattern->lefiSitePattern::name(), pattern->lefiSitePattern::x(),
                pattern->lefiSitePattern::y(),
                reader->orientStr(pattern->lefiSitePattern::orient()));
       }
     }
  }
  if (macro->hasSize()) {
    #ifdef LEF_CALLBACK_VERBOSITY
    printf("  SIZE %g BY %g ;\n", macro->sizeX(), macro->sizeY());
    #endif
    reader->macro_cursor()->set_width(macro->sizeX());
    reader->macro_cursor()->set_height(macro->sizeY());
  }
  if (macro->hasForeign()) {
    //BEG TODO-------------------------------------------------------------------------------------
    for (i = 0; i < macro->numForeigns(); i++) {
      #ifdef LEF_CALLBACK_VERBOSITY
      printf("  FOREIGN %s ", macro->foreignName(i));
      #endif
      if (macro->hasForeignPoint(i)) {
        #ifdef LEF_CALLBACK_VERBOSITY
        printf("( %g %g ) ", macro->foreignX(i), macro->foreignY(i));
        #endif
        if (macro->hasForeignOrient(i)) {
          #ifdef LEF_CALLBACK_VERBOSITY
          printf("%s ", macro->foreignOrientStr(i));
          #endif
        }
      }
      #ifdef LEF_CALLBACK_VERBOSITY
      printf(";\n");
      #endif
    }
    //END TODO-------------------------------------------------------------------------------------
  }
  if (macro->hasOrigin()) {
    #ifdef LEF_CALLBACK_VERBOSITY
    printf("  ORIGIN ( %g %g ) ;\n", macro->originX(), macro->originY());
    #endif
    reader->macro_cursor()->set_origin(macro->originX(), macro->originY());
  }
  if (macro->hasPower()) {
    printf("  POWER %g ;\n", macro->power());
  }
  propNum = macro->numProperties();
  if (propNum > 0) {
     printf("  PROPERTY ");
     for (i = 0; i < propNum; i++) {
        // value can either be a string or number
        if (macro->propValue(i)) {
           printf("%s %s ", macro->propName(i), macro->propValue(i));
        }
        else
           printf("%s %g ", macro->propName(i), macro->propNum(i));

        switch (macro->propType(i)) {
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
     printf(";\n");
  }
  //printf("END %s\n", macro->name());
  return 0;
}

// Function: macro_end_cbk
// MACRO macroName
//
// [CLASS
//
// { COVER [BUMP]
//
// | RING
//
// | BLOCK [BLACKBOX]
//
// | PAD [INPUT | OUTPUT |INOUT | POWER | SPACER | AREAIO]
//
// | CORE [FEEDTHRU | TIEHIGH | TIELOW | SPACER | ANTENNACELL]
//
// | ENDCAP {PRE | POST | TOPLEFT | TOPRIGHT | BOTTOMLEFT | BOTTOMRIGHT}
//
// }
//
// ;]
//
// [SOURCE {USER | BLOCK} ;]
//
// [FOREIGN foreignCellName [pt [orient]] ;] ...
//
// [ORIGIN pt ;]
//
// [SIZE width BY height ;]
//
// [SYMMETRY {X | Y | R90} ... ;]
//
// [SITE siteName ;]
//
// [PIN statement] ...
//
// [OBS statement] ...
//
int LEFReader::macro_end_cbk(lefrCallbackType_e c, const char* macroName, void* ud) {
  
  CHECK(c == lefrMacroEndCbkType);

  #ifdef LEF_CALLBACK_VERBOSITY
  printf("END %s\n",  macroName);
  #endif
  return 0;
}

// Function: pin_cbk
//
// PIN pinName
//
// FOREIGN foreignPinName [STRUCTURE [pt [orient] ] ] ;
//
// [DIRECTION {INPUT | OUTPUT [TRISTATE] | INOUT | FEEDTHRU} ;]
//
// [USE { SIGNAL | ANALOG | POWER | GROUND | CLOCK } ;]
//
// [SHAPE {ABUTMENT | RING | FEEDTHRU} ;]
//
// [MUSTJOIN pinName ;]
//
// {PORT
//
// [CLASS {NONE | CORE} ;]
//
// {layerGeometries} ...
//
// END} ...
//
// END pinName]
//
int LEFReader::pin_cbk(lefrCallbackType_e c, lefiPin* pin, void* ud) {

  CHECK(c == lefrPinCbkType);
  
  LEFReader* reader = (LEFReader*)ud;
  reader->_macropin_cursor = reader->macro_cursor()->insert_macropin(pin->name());

  int numPorts, i, j;
  lefiGeometries* geometry;
  lefiPinAntennaModel* aModel;
 
  #ifdef LEF_CALLBACK_VERBOSITY 
  printf("  PIN %s\n", pin->name());
  #endif
  if (pin->hasForeign()) {
    for (i = 0; i < pin->numForeigns(); i++) {
      if (pin->hasForeignOrient(i))
        printf("    FOREIGN %s STRUCTURE ( %g %g ) %s ;\n",
                pin->foreignName(i), pin->foreignX(i),
                pin->foreignY(i),
                pin->foreignOrientStr(i));
      else if (pin->hasForeignPoint(i))
        printf("    FOREIGN %s STRUCTURE ( %g %g ) ;\n", pin->foreignName(i), pin->foreignX(i), pin->foreignY(i));
      else
        printf("    FOREIGN %s ;\n", pin->foreignName(i));
    }
  }
  if (pin->hasLEQ())
    printf("    LEQ %s ;\n", pin->LEQ());
  if (pin->hasDirection()) {
    #ifdef LEF_CALLBACK_VERBOSITY 
    printf("    DIRECTION %s ;\n", pin->direction());
    #endif
    if(strcmp(pin->direction(), "OUTPUT") == 0) {
      reader->macropin_cursor()->set_direction(OUTPUT_MACROPIN_DIRECTION); 
    }
    else if(strcmp(pin->direction(), "INPUT") == 0) {
      reader->macropin_cursor()->set_direction(INPUT_MACROPIN_DIRECTION);
    }
    else {
      CHECK(false);
    }
  }
  if (pin->hasUse()) {
    #ifdef LEF_CALLBACK_VERBOSITY 
    printf("    USE %s ;\n", pin->use());
    #endif
    if(strcmp(pin->use(), "SIGNAL") == 0) {
      reader->macropin_cursor()->set_use(SIGNAL_PIN_USE);
    }
    else if(strcmp(pin->use(), "ANALOG") == 0) {
      reader->macropin_cursor()->set_use(ANALOG_PIN_USE);
    }
    else if(strcmp(pin->use(), "POWER") == 0) {
      reader->macropin_cursor()->set_use(POWER_PIN_USE);
    }
    else if(strcmp(pin->use(), "GROUND") == 0) {
      reader->macropin_cursor()->set_use(GROUND_PIN_USE);
    }
    else if(strcmp(pin->use(), "CLOCK") == 0) {
      reader->macropin_cursor()->set_use(CLOCK_PIN_USE);
    }
    else {
      CHECK(false);
    }
  }
  if (pin->hasShape())
     printf("    SHAPE %s ;\n", pin->shape());
  if (pin->hasMustjoin())
     printf("    MUSTJOIN %s ;\n", pin->mustjoin());
  if (pin->hasOutMargin())
     printf("    OUTPUTNOISEMARGIN %g %g ;\n",
             pin->outMarginHigh(), pin->outMarginLow());
  if (pin->hasOutResistance())
     printf("    OUTPUTRESISTANCE %g %g ;\n",
             pin->outResistanceHigh(),
             pin->outResistanceLow());
  if (pin->hasInMargin())
     printf("    INPUTNOISEMARGIN %g %g ;\n",
             pin->inMarginHigh(), pin->inMarginLow());
  if (pin->hasPower())
     printf("    POWER %g ;\n", pin->power());
  if (pin->hasLeakage())
     printf("    LEAKAGE %g ;\n", pin->leakage());
  if (pin->hasMaxload())
     printf("    MAXLOAD %g ;\n", pin->maxload());
  if (pin->hasCapacitance())
     printf("    CAPACITANCE %g ;\n", pin->capacitance());
  if (pin->hasResistance())
     printf("    RESISTANCE %g ;\n", pin->resistance());
  if (pin->hasPulldownres())
     printf("    PULLDOWNRES %g ;\n", pin->pulldownres());
  if (pin->hasTieoffr())
     printf("    TIEOFFR %g ;\n", pin->tieoffr());
  if (pin->hasVHI())
     printf("    VHI %g ;\n", pin->VHI());
  if (pin->hasVLO())
     printf("    VLO %g ;\n", pin->VLO());
  if (pin->hasRiseVoltage())
     printf("    RISEVOLTAGETHRESHOLD %g ;\n",
             pin->riseVoltage());
  if (pin->hasFallVoltage())
     printf("    FALLVOLTAGETHRESHOLD %g ;\n",
             pin->fallVoltage());
  if (pin->hasRiseThresh())
     printf("    RISETHRESH %g ;\n", pin->riseThresh());
  if (pin->hasFallThresh())
     printf("    FALLTHRESH %g ;\n", pin->fallThresh());
  if (pin->hasRiseSatcur())
     printf("    RISESATCUR %g ;\n", pin->riseSatcur());
  if (pin->hasFallSatcur())
     printf("    FALLSATCUR %g ;\n", pin->fallSatcur());
  if (pin->hasRiseSlewLimit())
     printf("    RISESLEWLIMIT %g ;\n", pin->riseSlewLimit());
  if (pin->hasFallSlewLimit())
     printf("    FALLSLEWLIMIT %g ;\n", pin->fallSlewLimit());
  if (pin->hasCurrentSource())
     printf("    CURRENTSOURCE %s ;\n", pin->currentSource());
  if (pin->hasTables())
     printf("    IV_TABLES %s %s ;\n", pin->tableHighName(), pin->tableLowName());
  if (pin->hasTaperRule())
     printf("    TAPERRULE %s ;\n", pin->taperRule());
  if (pin->hasNetExpr())
     printf("    NETEXPR \"%s\" ;\n", pin->netExpr());
  if (pin->hasSupplySensitivity())
     printf("    SUPPLYSENSITIVITY %s ;\n", pin->supplySensitivity());
  if (pin->hasGroundSensitivity())
     printf("    GROUNDSENSITIVITY %s ;\n", pin->groundSensitivity());
  if (pin->hasAntennaSize()) {
     for (i = 0; i < pin->numAntennaSize(); i++) {
        printf("    ANTENNASIZE %g ", pin->antennaSize(i));
        if (pin->antennaSizeLayer(i))
           printf("LAYER %s ", pin->antennaSizeLayer(i));
        printf(";\n");
     }
  }
  if (pin->hasAntennaMetalArea()) {
     for (i = 0; i < pin->numAntennaMetalArea(); i++) {
        printf("    ANTENNAMETALAREA %g ", pin->antennaMetalArea(i));
        if (pin->antennaMetalAreaLayer(i))
           printf("LAYER %s ", pin->antennaMetalAreaLayer(i));
        printf(";\n");
     }
  }
  if (pin->hasAntennaMetalLength()) {
     for (i = 0; i < pin->numAntennaMetalLength(); i++) {
        printf("    ANTENNAMETALLENGTH %g ",
           pin->antennaMetalLength(i));
        if (pin->antennaMetalLengthLayer(i))
           printf("LAYER %s ", pin->antennaMetalLengthLayer(i));
        printf(";\n");
     }
  }

  if (pin->hasAntennaPartialMetalArea()) {
     for (i = 0; i < pin->numAntennaPartialMetalArea(); i++) {
        printf("    ANTENNAPARTIALMETALAREA %g ",
                pin->antennaPartialMetalArea(i));
        if (pin->antennaPartialMetalAreaLayer(i))
           printf("LAYER %s ",
                   pin->antennaPartialMetalAreaLayer(i));
        printf(";\n");
     }
  }

  if (pin->hasAntennaPartialMetalSideArea()) {
     for (i = 0; i < pin->numAntennaPartialMetalSideArea(); i++) {
        printf("    ANTENNAPARTIALMETALSIDEAREA %g ",
                pin->antennaPartialMetalSideArea(i));
        if (pin->antennaPartialMetalSideAreaLayer(i))
           printf("LAYER %s ",
                   pin->antennaPartialMetalSideAreaLayer(i));
        printf(";\n");
     }
  }

  if (pin->hasAntennaPartialCutArea()) {
     for (i = 0; i < pin->numAntennaPartialCutArea(); i++) {
        printf("    ANTENNAPARTIALCUTAREA %g ",
                pin->antennaPartialCutArea(i));
        if (pin->antennaPartialCutAreaLayer(i))
           printf("LAYER %s ",
                   pin->antennaPartialCutAreaLayer(i));
        printf(";\n");
     }
  }

  if (pin->hasAntennaDiffArea()) {
     for (i = 0; i < pin->numAntennaDiffArea(); i++) {
        printf("    ANTENNADIFFAREA %g ",
                pin->antennaDiffArea(i));
        if (pin->antennaDiffAreaLayer(i))
           printf("LAYER %s ", pin->antennaDiffAreaLayer(i));
        printf(";\n");
     }
  }

  for (j = 0; j < pin->numAntennaModel(); j++) {
     aModel = pin->antennaModel(j); 
    
     printf("    ANTENNAMODEL %s ;\n", aModel->lefiPinAntennaModel::antennaOxide());

     if (aModel->lefiPinAntennaModel::hasAntennaGateArea()) {
        for (i = 0; i < aModel->lefiPinAntennaModel::numAntennaGateArea(); i++)
        {
           printf("    ANTENNAGATEAREA %g ", aModel->lefiPinAntennaModel::antennaGateArea(i));
           if (aModel->lefiPinAntennaModel::antennaGateAreaLayer(i))
              printf("LAYER %s ", aModel->lefiPinAntennaModel::antennaGateAreaLayer(i));
           printf(";\n");
        }
     }

     if (aModel->lefiPinAntennaModel::hasAntennaMaxAreaCar()) {
        for (i = 0; i < aModel->lefiPinAntennaModel::numAntennaMaxAreaCar();
           i++) {
           printf("    ANTENNAMAXAREACAR %g ", aModel->lefiPinAntennaModel::antennaMaxAreaCar(i));
           if (aModel->lefiPinAntennaModel::antennaMaxAreaCarLayer(i))
              printf("LAYER %s ", aModel->lefiPinAntennaModel::antennaMaxAreaCarLayer(i));
           printf(";\n");
        }
     }

     if (aModel->lefiPinAntennaModel::hasAntennaMaxSideAreaCar()) {
        for (i = 0; i < aModel->lefiPinAntennaModel::numAntennaMaxSideAreaCar();
           i++) {
           printf("    ANTENNAMAXSIDEAREACAR %g ", aModel->lefiPinAntennaModel::antennaMaxSideAreaCar(i));
           if (aModel->lefiPinAntennaModel::antennaMaxSideAreaCarLayer(i))
              printf("LAYER %s ", aModel->lefiPinAntennaModel::antennaMaxSideAreaCarLayer(i));
           printf(";\n");
        }
     }

     if (aModel->lefiPinAntennaModel::hasAntennaMaxCutCar()) {
        for (i = 0; i < aModel->lefiPinAntennaModel::numAntennaMaxCutCar(); i++)
        {
           printf("    ANTENNAMAXCUTCAR %g ", aModel->lefiPinAntennaModel::antennaMaxCutCar(i));
           if (aModel->lefiPinAntennaModel::antennaMaxCutCarLayer(i))
              printf("LAYER %s ", aModel->lefiPinAntennaModel::antennaMaxCutCarLayer(i));
           printf(";\n");
        }
     }
  }

  if (pin->numProperties() > 0) {
     printf("    PROPERTY ");
     for (i = 0; i < pin->numProperties(); i++) {
        // value can either be a string or number
        if (pin->propValue(i)) {
           printf("%s %s ", pin->propName(i), pin->propValue(i));
        }
        else
           printf("%s %g ", pin->propName(i), pin->propNum(i));
        switch (pin->propType(i)) {
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
     printf(";\n");
  }
  
  numPorts = pin->numPorts();
  for (i = 0; i < numPorts; i++) {
     #ifdef LEF_CALLBACK_VERBOSITY
     printf("    PORT\n");
     #endif

     geometry = pin->port(i);
     reader->prtGeometry(geometry);

     #ifdef LEF_CALLBACK_VERBOSITY
     printf("    END\n");
     #endif
  }

  #ifdef LEF_CALLBACK_VERBOSITY
  printf("  END %s\n", pin->name());
  #endif
  return 0;  
}


};

