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
//  $Revision: #8 $
//  $Date: 2015/01/27 $
//  $State:  $
// *****************************************************************************
// *****************************************************************************

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lex.h"
#include "defiDebug.hpp"
#include "defiMisc.hpp"

BEGIN_LEFDEF_PARSER_NAMESPACE

////////////////////////////////////////////////////
//
//    Handle points for a polygon
//
////////////////////////////////////////////////////
defiGeometries::defiGeometries() {
    pointsAllocated_ = 0;
    numPoints_ = 0;
}

void defiGeometries::Init() {
    Destroy();
}

void defiGeometries::Destroy() {
  if (pointsAllocated_) {
    defFree((char*)(x_));
    defFree((char*)(y_));
  }
  pointsAllocated_ = 0;
  numPoints_ = 0;
}

defiGeometries::~defiGeometries() {
  Destroy();
}

void defiGeometries::Reset() {
  numPoints_ = 0;
}

void defiGeometries::startList(int x, int y) {
  if (pointsAllocated_ == 0) {
    pointsAllocated_ = 16;
    x_ = (int*)defMalloc(sizeof(int)*16);
    y_ = (int*)defMalloc(sizeof(int)*16);
    numPoints_ = 0;
  } else   // reset the numPoints to 0
    numPoints_ = 0;
  addToList(x, y);
}

void defiGeometries::addToList(int x, int y) {
  if (numPoints_ == pointsAllocated_) {
    int i;
    int* nx;
    int* ny;
    pointsAllocated_ *= 2;
    nx = (int*)defMalloc(sizeof(int)*pointsAllocated_);
    ny = (int*)defMalloc(sizeof(int)*pointsAllocated_);
    for (i = 0; i < numPoints_; i++) {
      nx[i] = x_[i];
      ny[i] = y_[i];
    }
    defFree((char*)(x_));
    defFree((char*)(y_));
    x_ = nx;
    y_ = ny;
  }
  x_[numPoints_] = x;
  y_[numPoints_] = y;
  numPoints_ += 1;
}

int defiGeometries::numPoints() const {
  return numPoints_;
}

void defiGeometries::points(int index, int* x, int* y) const {
  char msg[160];
  if ((index < 0) || (index >= numPoints_)) {
     sprintf (msg, "ERROR (LEFPARS-6070): The index number %d given for GEOMETRY POINTS is invalid.\nValid index is from 0 to %d", index, numPoints_);
     defiError (0, 6070, msg);
     return;
  }
  *x = x_[index];
  *y = y_[index];
  return;
}

////////////////////////////////////////////////////
//
//    defiStyles
//
////////////////////////////////////////////////////

defiStyles::defiStyles() {
  Init();
}

void defiStyles::Init() {
  styleNum_ = 0;
  polygon_ = 0;
}

defiStyles::~defiStyles() {
  Destroy();
}

void defiStyles::Destroy() {
 clear();
}

void defiStyles::clear() {
  struct defiPoints* p;

  p = polygon_;
  if (p) {
    defFree((char*)(p->x));
    defFree((char*)(p->y));
    defFree((char*)(polygon_));
  }
  styleNum_ = 0;
  polygon_ = 0;
}

void defiStyles::setStyle(int styleNum) {
  styleNum_ = styleNum;
}

void defiStyles::setPolygon(defiGeometries* geom) {
  struct defiPoints* p;
  int i, x, y;

  if (polygon_ == 0) {
    p = (struct defiPoints*)defMalloc(sizeof(struct defiPoints));
    p->numPoints = geom->numPoints();
    p->x = (int*)defMalloc(sizeof(int)*p->numPoints);
    p->y = (int*)defMalloc(sizeof(int)*p->numPoints);
    numPointAlloc_ = p->numPoints; // keep track the max number pts
  } else if (numPointAlloc_ < geom->numPoints()) {
    // the incoming polygon has more number then has been allocated,
    // need to reallocate more memory
    p = polygon_;
    defFree((char*)(p->x));
    defFree((char*)(p->y));
    p->numPoints = geom->numPoints();
    p->x = (int*)defMalloc(sizeof(int)*p->numPoints);
    p->y = (int*)defMalloc(sizeof(int)*p->numPoints);
    numPointAlloc_ = p->numPoints; // keep track the max number pts
  } else {
    p = polygon_;
    p->numPoints = geom->numPoints();
  }
  for (i = 0; i < p->numPoints; i++) {
    geom->points(i, &x, &y);
    p->x[i] = x;
    p->y[i] = y;
  }
  polygon_ = p;
}

int defiStyles::style() const {
  return styleNum_;
}

struct defiPoints defiStyles::getPolygon() const {
  return *(polygon_);
}
END_LEFDEF_PARSER_NAMESPACE

