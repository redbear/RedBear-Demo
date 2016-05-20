//
//  NanoView.h
//  RedBear8Nanos
//
//  Created by Sunny Cheung on 28/11/2015.
//  Copyright © 2015 khl. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "RedBear8Nanos-Swift.h"
#import "Nano.h"
#import <FontAwesomeKit/FAKFontAwesome.h>

@interface NanoView : UIView
@property (weak, nonatomic) IBOutlet UISegmentedControl *status;
@property (weak, nonatomic) IBOutlet GradientSlider *slider;
@property (weak, nonatomic) IBOutlet UILabel *deviceNo;
@property (nonatomic) Nano *nano;
-(void) updateView;


@end
