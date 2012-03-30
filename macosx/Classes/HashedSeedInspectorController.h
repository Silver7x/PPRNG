/*
  Copyright (C) 2011-2012 chiizu
  chiizu.pprng@gmail.com
  
  This file is part of PPRNG.
  
  PPRNG is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  PPRNG is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with PPRNG.  If not, see <http://www.gnu.org/licenses/>.
*/



#import <Cocoa/Cocoa.h>
#import "VertResizeOnlyWindowController.h"
#import "Gen5ConfigurationController.h"
#import "HashedSeedInspectorFramesTabController.h"
#import "HashedSeedInspectorAdjacentsTabController.h"
#import "HashedSeedInspectorEggsTabController.h"
#import "SearchResultProtocols.h"

#include "HashedSeed.h"

@interface HashedSeedInspectorController : VertResizeOnlyWindowController
{
  IBOutlet Gen5ConfigurationController  *gen5ConfigController;
  
  IBOutlet HashedSeedInspectorFramesTabController     *framesTabController;
  
  IBOutlet HashedSeedInspectorAdjacentsTabController  *adjacentsTabController;
  
  IBOutlet HashedSeedInspectorEggsTabController       *eggsTabController;
  
  NSDate    *startDate;
  NSNumber  *startHour, *startMinute, *startSecond;
  NSNumber  *timer0, *vcount, *vframe;
  
  uint32_t  button1, button2, button3;
  
  NSNumber  *rawSeed;
  NSNumber  *initialPIDFrame;
  
  NSString  *selectedTabId;
}

@property (copy) NSDate    *startDate;
@property (copy) NSNumber  *startHour, *startMinute, *startSecond;
@property (copy) NSNumber  *timer0, *vcount, *vframe;

@property        uint32_t  button1, button2, button3;

@property (copy) NSNumber  *rawSeed;
@property (copy) NSNumber  *initialPIDFrame;

@property (copy) NSString  *selectedTabId;

@property (readonly)
  HashedSeedInspectorFramesTabController *framesTabController;
@property (readonly)
  HashedSeedInspectorAdjacentsTabController *adjacentsTabController;
@property (readonly)
  HashedSeedInspectorEggsTabController *eggsTabController;

- (IBAction) seedParameterChanged:(id)sender;
- (IBAction) seedValueChanged:(id)sender;

- (void)setSeedFromResult:(id <HashedSeedResultParameters>)result;
- (void)setSeed:(const pprng::HashedSeed&)seed;

@end
