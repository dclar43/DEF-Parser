// *****************************************************************************
// *****************************************************************************
// Copyright 2013, Cadence Design Systems
// 
// This  file  is  part  of  the  Cadence  LEF/DEF  Open   Source
// Distribution,  Product Version 5.8. 
// 
// Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
// 
//        http://www.apache.org/licenses/LICENSE-2.0
// 
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
//    implied. See the License for the specific language governing
//    permissions and limitations under the License.
// 
// For updates, support, or to become part of the LEF/DEF Community,
// check www.openeda.org for details.
// 
//  $Author: dell $
//  $Revision: #7 $
//  $Date: 2015/01/27 $
//  $State:  $
// *****************************************************************************
// *****************************************************************************

#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "defiNonDefault.hpp"
#include "defiDebug.hpp"

BEGIN_LEFDEF_PARSER_NAMESPACE

/////////////////////////////////////////
/////////////////////////////////////////
//
//     defiNonDefault
//
/////////////////////////////////////////
/////////////////////////////////////////

defiNonDefault::defiNonDefault() { 
  Init();
}

void defiNonDefault::Init() { 
  name_ = 0;
  hardSpacing_ = 0;
  numLayers_ = 0;
  width_ = 0;
  hasDiagWidth_ = 0;
  hasSpacing_ = 0;
  hasWireExt_ = 0;
  numVias_ = 0;
  viasAllocated_ = 0;
  viaNames_ = 0;
  numViaRules_ = 0;
  viaRulesAllocated_ = 0;
  viaRuleNames_ = 0;
  numMinCuts_ = 0;
  minCutsAllocated_ = 0;
  cutLayerName_ = 0;
  numCuts_ = 0;
  numProps_ = 0;
  propsAllocated_ = 0;
  names_ = 0;
  values_ = 0;
  dvalues_ = 0;
  types_ = 0;
  layersAllocated_ = 0;
  layerName_ = 0;
  width_ = 0;
  hasDiagWidth_ = 0;
  diagWidth_ = 0;
  hasSpacing_ = 0;
  spacing_ = 0;
  hasWireExt_ = 0;
  wireExt_ = 0;
}

defiNonDefault::~defiNonDefault() { 
  Destroy();
}

void defiNonDefault::Destroy() {
  clear();
  if (propsAllocated_) {
    defFree((char*)(names_));
    defFree((char*)(values_));
    defFree((char*)(dvalues_));
    defFree((char*)(types_));
  }
  if (layersAllocated_) {
    defFree((char*)(layerName_));
    defFree((char*)(width_));
    defFree((char*)(hasDiagWidth_));
    defFree((char*)(diagWidth_));
    defFree((char*)(hasSpacing_));
    defFree((char*)(spacing_));
    defFree((char*)(hasWireExt_));
    defFree((char*)(wireExt_));
  }
  if (viasAllocated_)
    defFree((char*)(viaNames_));
  if (viaRulesAllocated_)
    defFree((char*)(viaRuleNames_));
  if (minCutsAllocated_) {
    defFree((char*)(cutLayerName_));
    defFree((char*)(numCuts_));
  }
}

void defiNonDefault::clear() { 
  int i;

  if (name_)
    defFree(name_);
  hardSpacing_ = 0;
  for (i = 0; i < numProps_; i++) {
    defFree(names_[i]);
    defFree(values_[i]);
    dvalues_[i] = 0;
  }
  numProps_ = 0;
  for (i = 0; i < numLayers_; i++)
    defFree(layerName_[i]);
  numLayers_ = 0;
  for (i = 0; i < numVias_; i++)
    defFree((char*)(viaNames_[i]));
  numVias_ = 0;
  for (i = 0; i < numViaRules_; i++)
    defFree((char*)(viaRuleNames_[i]));
  numViaRules_ = 0;
  for (i = 0; i < numMinCuts_; i++)
    defFree((char*)(cutLayerName_[i]));
  numMinCuts_ = 0;
}

void defiNonDefault::setName(const char* name) { 
  name_ = (char*)defMalloc(strlen(name)+1);
  strcpy(name_, DEFCASE(name));
}

void defiNonDefault::setHardspacing() {
  hardSpacing_ = 1;
}

void defiNonDefault::addLayer(const char* name) { 
  int len = strlen(name) + 1;
  if (numLayers_ == layersAllocated_) {
    int i;
    char**  newl;
    double* neww;
    double* newd;
    double* news;
    double* newe;
    char*   newhd;
    char*   newhs;
    char*   newhe;

    if (layersAllocated_ == 0)
      layersAllocated_ = 2;
    else
      layersAllocated_ *= 2;
    newl = (char**)defMalloc(sizeof(char*) * layersAllocated_);
    newe = (double*)defMalloc(sizeof(double) * layersAllocated_);
    neww = (double*)defMalloc(sizeof(double) * layersAllocated_);
    newd = (double*)defMalloc(sizeof(double) * layersAllocated_);
    news = (double*)defMalloc(sizeof(double) * layersAllocated_);
    newhe = (char*)defMalloc(sizeof(char) * layersAllocated_);
    newhd = (char*)defMalloc(sizeof(char) * layersAllocated_);
    newhs = (char*)defMalloc(sizeof(char) * layersAllocated_);
    for (i = 0; i < numLayers_; i++) {
      newl[i]  = layerName_[i];
      neww[i]  = width_[i];
      newd[i]  = diagWidth_[i];
      news[i]  = spacing_[i];
      newe[i]  = wireExt_[i];
      newhe[i]  = hasWireExt_[i];
      newhd[i]  = hasDiagWidth_[i];
      newhs[i]  = hasSpacing_[i];
    }
    if (layersAllocated_ > 2) {
      defFree((char*)(layerName_));
      defFree((char*)(width_));
      defFree((char*)(diagWidth_));
      defFree((char*)(spacing_));
      defFree((char*)(wireExt_));
      defFree((char*)(hasWireExt_));
      defFree((char*)(hasDiagWidth_));
      defFree((char*)(hasSpacing_));
    }
    layerName_ = newl;
    width_ = neww;
    diagWidth_ = newd;
    spacing_ = news;
    wireExt_ = newe;
    hasDiagWidth_ = newhd;
    hasSpacing_ = newhs;
    hasWireExt_ = newhe;
  }
  layerName_[numLayers_] = (char*)defMalloc(len);
  strcpy(layerName_[numLayers_], DEFCASE(name));
  width_[numLayers_] = 0.0;
  diagWidth_[numLayers_] = 0.0;
  spacing_[numLayers_] = 0.0;
  wireExt_[numLayers_] = 0.0;
  hasDiagWidth_[numLayers_] = '\0';
  hasSpacing_[numLayers_] = '\0';
  hasWireExt_[numLayers_] = '\0';
  numLayers_ += 1;
}

void defiNonDefault::addWidth(double num) { 
  width_[numLayers_-1] = num;
}

void defiNonDefault::addDiagWidth(double num) { 
  diagWidth_[numLayers_-1] = num;
  hasDiagWidth_[numLayers_-1] = 1;
}

void defiNonDefault::addSpacing(double num) { 
  spacing_[numLayers_-1] = num;
  hasSpacing_[numLayers_-1] = 1;
}

void defiNonDefault::addWireExt(double num) { 
  wireExt_[numLayers_-1] = num;
  hasWireExt_[numLayers_-1] = 1;
}

void defiNonDefault::addVia(const char* name) {
  if (numVias_ == viasAllocated_) {
    int i;
    char** vn;

    if (viasAllocated_ == 0)
      viasAllocated_ = 2;
    else
      viasAllocated_ *= 2;
    vn = (char**)defMalloc(sizeof(char*)* viasAllocated_);
    for (i = 0; i < numVias_; i++) {
      vn[i] = viaNames_[i];
    }
    defFree((char*)(viaNames_));
    viaNames_ = vn;
  } 
  viaNames_[numVias_] = (char*)defMalloc(strlen(name)+1);
  strcpy(viaNames_[numVias_], DEFCASE(name));
  numVias_ += 1;
}

void defiNonDefault::addViaRule(const char* name) {
  if (numViaRules_ == viaRulesAllocated_) {
    int i;
    char** vn;

    if (viaRulesAllocated_ == 0)
      viaRulesAllocated_ = 2;
    else
      viaRulesAllocated_ *= 2;
    vn = (char**)defMalloc(sizeof(char*)* viaRulesAllocated_);
    for (i = 0; i < numViaRules_; i++) {
      vn[i] = viaRuleNames_[i];
    }
    defFree((char*)(viaRuleNames_));
    viaRuleNames_ = vn;
  } 
  viaRuleNames_[numViaRules_] = (char*)defMalloc(strlen(name)+1);
  strcpy(viaRuleNames_[numViaRules_], DEFCASE(name));
  numViaRules_ += 1;
}

void defiNonDefault::addMinCuts(const char* name, int numCuts) {
  if (numMinCuts_ == minCutsAllocated_) {
    int i;
    char** cln;
    int*   nc;

    if (minCutsAllocated_ == 0)
      minCutsAllocated_ = 2;
    else
      minCutsAllocated_ *= 2;
    cln = (char**)defMalloc(sizeof(char*)* minCutsAllocated_);
    nc = (int*)defMalloc(sizeof(int)* minCutsAllocated_);
    for (i = 0; i < numMinCuts_; i++) {
      cln[i] = cutLayerName_[i];
      nc[i]  = numCuts_[i];
    }
    if (minCutsAllocated_ > 2) {
      defFree((char*)(cutLayerName_));
      defFree((char*)(numCuts_));
    }
    cutLayerName_ = cln;
    numCuts_ = nc;
  } 
  cutLayerName_[numMinCuts_] = (char*)defMalloc(strlen(name)+1);
  strcpy(cutLayerName_[numMinCuts_], DEFCASE(name));
  numCuts_[numMinCuts_] = numCuts;
  numMinCuts_ += 1;
}

const char* defiNonDefault::name() const {
  return name_;
}

int defiNonDefault::hasHardspacing() const {
  return hardSpacing_;
}

int defiNonDefault::numLayers() const { 
  return numLayers_;
}

const char* defiNonDefault::layerName(int index) const { 
  char msg[160];
  if (index < 0 || index >= numLayers_) {
     sprintf (msg, "ERROR (DEFPARS-6090): The index number %d specified for the NONDEFAULT LAYER is invalid.\nValid index is from 0 to %d. Specify a valid index number and then try again.",
             index, numLayers_);
     defiError (0, 6090, msg);
     return 0;
  }
  return layerName_[index];
}

// Will be obsoleted in 5.7
double defiNonDefault::layerWidth(int index) const { 
  char msg[160];
  if (index < 0 || index >= numLayers_) {
     sprintf (msg, "ERROR (DEFPARS-6090): The index number %d specified for the NONDEFAULT LAYER is invalid.\nValid index is from 0 to %d. Specify a valid index number and then try again.",
             index, numLayers_);
     defiError (0, 6090, msg);
     return 0;
  }
  return width_[index];
}

int defiNonDefault::layerWidthVal(int index) const { 
  char msg[160];
  if (index < 0 || index >= numLayers_) {
     sprintf (msg, "ERROR (DEFPARS-6090): The index number %d specified for the NONDEFAULT LAYER is invalid.\nValid index is from 0 to %d. Specify a valid index number and then try again.",
             index, numLayers_);
     defiError (0, 6090, msg);
     return 0;
  }
  return (int)width_[index];
}

int defiNonDefault::hasLayerDiagWidth(int index) const { 
  char msg[160];
  if (index < 0 || index >= numLayers_) {
     sprintf (msg, "ERROR (DEFPARS-6090): The index number %d specified for the NONDEFAULT LAYER is invalid.\nValid index is from 0 to %d. Specify a valid index number and then try again.",
             index, numLayers_);
     defiError (0, 6090, msg);
     return 0;
  }
  return hasDiagWidth_[index];
}

// Will be obsoleted in 5.7
double defiNonDefault::layerDiagWidth(int index) const { 
  char msg[160];
  if (index < 0 || index >= numLayers_) {
     sprintf (msg, "ERROR (DEFPARS-6090): The index number %d specified for the NONDEFAULT LAYER is invalid.\nValid index is from 0 to %d. Specify a valid index number and then try again.",
             index, numLayers_);
     defiError (0, 6090, msg);
     return 0;
  }
  return diagWidth_[index];
}

int defiNonDefault::layerDiagWidthVal(int index) const { 
  char msg[160];
  if (index < 0 || index >= numLayers_) {
     sprintf (msg, "ERROR (DEFPARS-6090): The index number %d specified for the NONDEFAULT LAYER is invalid.\nValid index is from 0 to %d. Specify a valid index number and then try again.",
             index, numLayers_);
     defiError (0, 6090, msg);
     return 0;
  }
  return (int)diagWidth_[index];
}

int defiNonDefault::hasLayerWireExt(int index) const { 
  char msg[160];
  if (index < 0 || index >= numLayers_) {
     sprintf (msg, "ERROR (DEFPARS-6090): The index number %d specified for the NONDEFAULT LAYER is invalid.\nValid index is from 0 to %d. Specify a valid index number and then try again.",
             index, numLayers_);
     defiError (0, 6090, msg);
     return 0;
  }
  return hasWireExt_[index];
}

int defiNonDefault::hasLayerSpacing(int index) const { 
  char msg[160];
  if (index < 0 || index >= numLayers_) {
     sprintf (msg, "ERROR (DEFPARS-6090): The index number %d specified for the NONDEFAULT LAYER is invalid.\nValid index is from 0 to %d. Specify a valid index number and then try again.",
             index, numLayers_);
     defiError (0, 6090, msg);
     return 0;
  }
  return hasSpacing_[index];
}

// Will be obsoleted in 5.7
double defiNonDefault::layerWireExt(int index) const { 
  char msg[160];
  if (index < 0 || index >= numLayers_) {
     sprintf (msg, "ERROR (DEFPARS-6090): The index number %d specified for the NONDEFAULT LAYER is invalid.\nValid index is from 0 to %d. Specify a valid index number and then try again.",
             index, numLayers_);
     defiError (0, 6090, msg);
     return 0;
  }
  return wireExt_[index];
}

int defiNonDefault::layerWireExtVal(int index) const { 
  char msg[160];
  if (index < 0 || index >= numLayers_) {
     sprintf (msg, "ERROR (DEFPARS-6090): The index number %d specified for the NONDEFAULT LAYER is invalid.\nValid index is from 0 to %d. Specify a valid index number and then try again.",
             index, numLayers_);
     defiError (0, 6090, msg);
     return 0;
  }
  return (int)wireExt_[index];
}

// Will be obsoleted in 5.7
double defiNonDefault::layerSpacing(int index) const { 
  char msg[160];
  if (index < 0 || index >= numLayers_) {
     sprintf (msg, "ERROR (DEFPARS-6090): The index number %d specified for the NONDEFAULT LAYER is invalid.\nValid index is from 0 to %d. Specify a valid index number and then try again.",
             index, numLayers_);
     defiError (0, 6090, msg);
     return 0;
  }
  return spacing_[index];
}

int defiNonDefault::layerSpacingVal(int index) const { 
  char msg[160];
  if (index < 0 || index >= numLayers_) {
     sprintf (msg, "ERROR (DEFPARS-6090): The index number %d specified for the NONDEFAULT LAYER is invalid.\nValid index is from 0 to %d. Specify a valid index number and then try again.",
             index, numLayers_);
     defiError (0, 6090, msg);
     return 0;
  }
  return (int)spacing_[index];
}

int defiNonDefault::numVias() const { 
  return numVias_;
}

const char* defiNonDefault::viaName(int index) const {
  char msg[160];
  if (index < 0 || index >= numVias_) {
     sprintf (msg, "ERROR (DEFPARS-6090): The index number %d specified for the NONDEFAULT LAYER is invalid.\nValid index is from 0 to %d. Specify a valid index number and then try again.",
             index, numLayers_);
     defiError (0, 6090, msg);
     return 0;
  }
  return viaNames_[index];
}

int defiNonDefault::numViaRules() const { 
  return numViaRules_;
}

const char* defiNonDefault::viaRuleName(int index) const {
  char msg[160];
  if (index < 0 || index >= numViaRules_) {
     sprintf (msg, "ERROR (DEFPARS-6090): The index number %d specified for the NONDEFAULT LAYER is invalid.\nValid index is from 0 to %d. Specify a valid index number and then try again.",
             index, numLayers_);
     defiError (0, 6090, msg);
     return 0;
  }
  return viaRuleNames_[index];
}

int defiNonDefault::numMinCuts() const {
  return numMinCuts_;
}

const char* defiNonDefault::cutLayerName(int index) const {
  char msg[160];
  if (index < 0 || index >= numMinCuts_) {
     sprintf (msg, "ERROR (DEFPARS-6090): The index number %d specified for the NONDEFAULT LAYER is invalid.\nValid index is from 0 to %d. Specify a valid index number and then try again.",
             index, numLayers_);
     defiError (0, 6090, msg);
     return 0;
  }
  return cutLayerName_[index];
}

int defiNonDefault::numCuts(int index) const {
  char msg[160];
  if (index < 0 || index >= numMinCuts_) {
     sprintf (msg, "ERROR (DEFPARS-6090): The index number %d specified for the NONDEFAULT LAYER is invalid.\nValid index is from 0 to %d. Specify a valid index number and then try again.",
             index, numLayers_);
     defiError (0, 6090, msg);
     return 0;
  }
  return numCuts_[index];
}

void defiNonDefault::print(FILE* f) const { 
  int   i;

  fprintf(f, "nondefaultrule %s\n", name());
  
  fprintf(f, "%d layers   %d vias   %d viarules   %d mincuts\n",
    numLayers(),
    numVias(),
    numViaRules(),
    numMinCuts());

  for (i = 0; i < numLayers(); i++) {
    fprintf(f, "  Layer %s\n", layerName(i));
    fprintf(f, "    WIDTH %g\n", layerWidth(i));
    if (hasLayerDiagWidth(i))
      fprintf(f, "    DIAGWIDTH %g\n", layerDiagWidth(i));
    if (hasLayerSpacing(i))
      fprintf(f, "    SPACING %g\n", layerSpacing(i));
    if (hasLayerWireExt(i))
      fprintf(f, "    WIREEXT %g\n",
              layerWireExt(i));
  }
  for (i = 0; i < numVias(); i++) {
    fprintf(f, "    VIA %s\n", viaName(i));
  }
  for (i = 0; i < numViaRules(); i++) {
    fprintf(f, "    VIARULE %s\n", viaRuleName(i));
  }
  for (i = 0; i < numMinCuts(); i++) {
    fprintf(f, "    MINCUTS %s %d\n", cutLayerName(i),
            numCuts(i));
  }
}

int defiNonDefault::numProps() const {
  return numProps_;
}

void defiNonDefault::addProperty(const char* name, const char* value,
                                 const char type) {
  int len = strlen(name) + 1;
  if (numProps_ == propsAllocated_) {
    int i;
    int max;
    int lim = numProps_;
    char**  nn;
    char**  nv;
    double* nd;
    char*   nt;

    if (propsAllocated_ == 0) 
      max = propsAllocated_ = 2;
    else
      max = propsAllocated_ *= 2;
    nn = (char**)defMalloc(sizeof(char*) * max);
    nv = (char**)defMalloc(sizeof(char*) * max);
    nd = (double*)defMalloc(sizeof(double) * max);
    nt = (char*)defMalloc(sizeof(char) * max);
    for (i = 0; i < lim; i++) {
      nn[i] = names_[i];
      nv[i] = values_[i];
      nd[i] = dvalues_[i];
      nt[i] = types_[i];
    }
    defFree((char*)(names_));
    defFree((char*)(values_));
    defFree((char*)(dvalues_));
    defFree((char*)(types_));
    names_   = nn;
    values_  = nv;
    dvalues_ = nd;
    types_   = nt;
  }
  names_[numProps_] = (char*)defMalloc(sizeof(char)*len);
  strcpy(names_[numProps_],name);
  len = strlen(value) + 1;
  values_[numProps_] = (char*)defMalloc(sizeof(char) * len);
  strcpy(values_[numProps_],value);
  dvalues_[numProps_] = 0;
  types_[numProps_] = type;
  numProps_ += 1;
}

void defiNonDefault::addNumProperty(const char* name, const double d,
                                    const char* value, const char type) {
  int len = strlen(name) + 1;
  if (numProps_ == propsAllocated_) {
    int i;
    int max;
    int lim = numProps_;
    char**  nn;
    char**  nv;
    double* nd;
    char*   nt;

    if (propsAllocated_ == 0) 
      max = propsAllocated_ = 2;
    else
      max = propsAllocated_ *= 2;
    nn = (char**)defMalloc(sizeof(char*) * max);
    nv = (char**)defMalloc(sizeof(char*) * max);
    nd = (double*)defMalloc(sizeof(double) * max);
    nt = (char*)defMalloc(sizeof(char) * max);
    for (i = 0; i < lim; i++) {
      nn[i] = names_[i];
      nv[i] = values_[i];
      nd[i] = dvalues_[i];
      nt[i] = types_[i];
    }
    defFree((char*)(names_));
    defFree((char*)(values_));
    defFree((char*)(dvalues_));
    defFree((char*)(types_));
    names_   = nn;
    values_  = nv;
    dvalues_ = nd;
    types_   = nt;
  }
  names_[numProps_] = (char*)defMalloc(sizeof(char) * len);
  strcpy(names_[numProps_],name);
  len = strlen(value) + 1;
  values_[numProps_] = (char*)defMalloc(sizeof(char) * len);
  strcpy(values_[numProps_],value);
  dvalues_[numProps_] = d;
  types_[numProps_] = type;
  numProps_ += 1;
}

const char* defiNonDefault::propName(int index) const {
  char msg[160];
  if (index < 0 || index >= numProps_) {
     sprintf (msg, "ERROR (DEFPARS-6091): The index number %d specified for the NONDEFAULT PROPERTY is invalid.\nValid index is from 0 to %d. Specify a valid index number and then try again.",
             index, numProps_);
     defiError (0, 6091, msg);
     return 0;
  }
  return names_[index];
}

const char* defiNonDefault::propValue(int index) const {
  char msg[160];
  if (index < 0 || index >= numProps_) {
     sprintf (msg, "ERROR (DEFPARS-6091): The index number %d specified for the NONDEFAULT PROPERTY is invalid.\nValid index is from 0 to %d. Specify a valid index number and then try again.",
             index, numProps_);
     defiError (0, 6091, msg);
     return 0;
  }
  return values_[index];
}

double defiNonDefault::propNumber(int index) const {
  char msg[160];
  if (index < 0 || index >= numProps_) {
     sprintf (msg, "ERROR (DEFPARS-6091): The index number %d specified for the NONDEFAULT PROPERTY is invalid.\nValid index is from 0 to %d. Specify a valid index number and then try again.",
             index, numProps_);
     defiError (0, 6091, msg);
     return 0;
  }
  return dvalues_[index];
}

const char defiNonDefault::propType(int index) const {
  char msg[160];
  if (index < 0 || index >= numProps_) {
     sprintf (msg, "ERROR (DEFPARS-6091): The index number %d specified for the NONDEFAULT PROPERTY is invalid.\nValid index is from 0 to %d. Specify a valid index number and then try again.",
             index, numProps_);
     defiError (0, 6091, msg);
     return 0;
  }
  return types_[index];
}

int defiNonDefault::propIsNumber(int index) const {
  char msg[160];
  if (index < 0 || index >= numProps_) {
     sprintf (msg, "ERROR (DEFPARS-6091): The index number %d specified for the NONDEFAULT PROPERTY is invalid.\nValid index is from 0 to %d. Specify a valid index number and then try again.",
             index, numProps_);
     defiError (0, 6091, msg);
     return 0;
  }
  return dvalues_[index] ? 1 : 0;
}

int defiNonDefault::propIsString(int index) const {
  char msg[160];
  if (index < 0 || index >= numProps_) {
     sprintf (msg, "ERROR (DEFPARS-6091): The index number %d specified for the NONDEFAULT PROPERTY is invalid.\nValid index is from 0 to %d. Specify a valid index number and then try again.",
             index, numProps_);
     defiError (0, 6091, msg);
     return 0;
  }
  return dvalues_[index] ? 0 : 1;
}
END_LEFDEF_PARSER_NAMESPACE

