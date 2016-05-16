//
//  NanoViewController.h
//  RedBear8Nanos
//
//  Created by Sunny Cheung on 27/11/2015.
//  Copyright © 2015 khl. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "NanoView.h"

@interface NanoViewController : UIViewController
@property (nonatomic, weak)  NanoView *nano0;
@property (nonatomic, weak) NanoView *nano1;
@property (nonatomic, weak) NanoView *nano2;
@property (nonatomic, weak) NanoView *nano3;
@property (nonatomic, weak) NanoView *nano4;
@property (nonatomic, weak) NanoView *nano5;
@property (nonatomic, weak) NanoView *nano6;
@property (nonatomic, weak) NanoView *nano7;

@property (nonatomic) NSOutputStream *outputStream;
@end
