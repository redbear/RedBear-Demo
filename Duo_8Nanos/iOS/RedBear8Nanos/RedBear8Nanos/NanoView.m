//
//  NanoView.m
//  RedBear8Nanos
//
//  Created by Sunny Cheung on 28/11/2015.
//  Copyright © 2015 khl. All rights reserved.
//

#import "NanoView.h"

@implementation NanoView

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/


-(instancetype) initWithCoder:(NSCoder *)aDecoder
{
    self = [super initWithCoder:aDecoder];
 
    return self;
}

-(instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    return self;
}
- (IBAction)switch:(id)sender {
    
    NSNumber *red = [[NSNumber alloc] initWithFloat:0];
    NSNumber *green = [[NSNumber alloc] initWithFloat:0];
    NSNumber *blue = [[NSNumber alloc] initWithFloat:0];
    
    if (self.status.selectedSegmentIndex == 0) { // on
        CGFloat r, g, b, a;
        [[UIColor colorWithHue:self.slider.value/360 saturation:1.0 brightness:1.0 alpha:1.0] getRed:&r green:&g blue:&b alpha:&a];
    
        red = [[NSNumber alloc] initWithFloat:r*255];
        green = [[NSNumber alloc] initWithFloat:g*255];
        blue = [[NSNumber alloc] initWithFloat:b*255];
   
        [[NSNotificationCenter defaultCenter] postNotificationName:@"SETRGB" object:self userInfo:@{@"r":
                                                                                               red,
                                                                                                   @"g":
                                                                                                    green,
                                                                                                   @"b":
                                                                                                       blue,
                                                                                                    @"id":
                                                                                                        [[NSNumber alloc] initWithUnsignedInteger:_nano.identifier], @"op" : [[NSNumber alloc] initWithInt:1]}];
       [self.nano updateRed:red Green:green Blue:blue];
       
    }
    
    else {
       
        [[NSNotificationCenter defaultCenter] postNotificationName:@"SETRGB" object:self userInfo:@{@"r":
                                                                                                       red,
                                                                                                   @"g":
                                                                                                       green,
                                                                                                   @"b":
                                                                                                       blue,
                                                                                                    @"id":
                                                                                                        [[NSNumber alloc] initWithUnsignedInteger:_nano.identifier], @"op" : [[NSNumber alloc] initWithInt:1]}];
        [self.nano updateRed:red Green:green Blue:blue];
    }

     [self updateView];
}

- (IBAction)changeColor:(GradientSlider *)slider{
    if (slider.value == 0) {
        NSNumber *red = [[NSNumber alloc] initWithFloat:0];
        NSNumber *green = [[NSNumber alloc] initWithFloat:0];
        NSNumber *blue = [[NSNumber alloc] initWithFloat:0];
        [[NSNotificationCenter defaultCenter] postNotificationName:@"SETRGB" object:self userInfo:@{@"r":
                                                                                                        red,
                                                                                                    @"g":
                                                                                                        green,
                                                                                                    @"b":
                                                                                                        blue,
                                                                                                    @"id":
                                                                                                        [[NSNumber alloc] initWithUnsignedInteger:_nano.identifier], @"op" : [[NSNumber alloc] initWithInt:1] }];
        [self.nano updateRed:red Green:green Blue:blue];

    }
    else {
        CGFloat r, g, b, a;
        [[UIColor colorWithHue:self.slider.value/360 saturation:1.0 brightness:1.0 alpha:1.0] getRed:&r green:&g blue:&b alpha:&a];
        //        self.colorValueLabel.text = [NSString stringWithFormat:@"#%02x%02x%02x", (int)(r * 255), (int)(g * 255), (int)(b * 255) ];
        NSNumber *red = [[NSNumber alloc] initWithFloat:r*255];
        NSNumber *green = [[NSNumber alloc] initWithFloat:g*255];
        NSNumber *blue = [[NSNumber alloc] initWithFloat:b*255];
        [[NSNotificationCenter defaultCenter] postNotificationName:@"SETRGB" object:self userInfo:@{@"r":
                                                                                                        red,
                                                                                                    @"g":
                                                                                                        green,                                                                                                    @"b":
                                                                                                        blue,
                                                                                                    @"id":
                                                                                                        [[NSNumber alloc] initWithUnsignedInteger:_nano.identifier], @"op" : [[NSNumber alloc] initWithInt:1]}];
        [self.nano updateRed:red Green:green Blue:blue];
    }
    [self updateView];
}

-(void)setNano:(Nano *)nano
{
    _nano = nano;
    self.deviceNo.text = [NSString stringWithFormat:@"Nano %ld", _nano.identifier];
    [self.status setSelectedSegmentIndex:(_nano.status? 0 : 1)];
    CGFloat hue, saturation, brightness, alpha;
    UIColor *color = [UIColor colorWithRed:_nano.red/255.0 green:_nano.green/255.0 blue:_nano.blue/255.0 alpha:1.0];
    [color getHue:&hue saturation:&saturation brightness:&brightness alpha:&alpha];
    self.slider.value = hue * 360;
    self.slider.continuous = NO;
    self.backgroundColor = color;
    UIFont *font = [UIFont boldSystemFontOfSize:20.0f];
    NSDictionary *attributes = [NSDictionary dictionaryWithObject:font
                                                           forKey:NSFontAttributeName];
    [self.status setTitleTextAttributes:attributes
                               forState:UIControlStateNormal];
}

-(void) updateView
{
    [self.status setSelectedSegmentIndex:(_nano.status? 0 : 1)];
    CGFloat hue, saturation, brightness, alpha;
    UIColor *color = [UIColor colorWithRed:_nano.red/255.0 green:_nano.green/255.0 blue:_nano.blue/255.0 alpha:1.0];
    [color getHue:&hue saturation:&saturation brightness:&brightness alpha:&alpha];
    self.slider.value = hue * 360;
    self.backgroundColor = color;

    
}

@end
