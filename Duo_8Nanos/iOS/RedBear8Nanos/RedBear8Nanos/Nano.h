//
//  Nano.h
//  RedBear8Nanos
//
//  Created by Sunny Cheung on 27/11/2015.
//  Copyright © 2015 khl. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Nano : NSObject

@property(nonatomic, readonly) NSUInteger identifier;

@property (nonatomic, assign, readonly) BOOL status;
@property (nonatomic, assign, readonly) int red;
@property (nonatomic, assign, readonly) int green;
@property (nonatomic, assign, readonly) int blue;
-(instancetype)initWithDeviceNo:(NSUInteger) deviceNo;
-(NSData *) readStatus;
-(void) updateRed:(NSNumber *) red Green:(NSNumber *) green Blue:(NSNumber *) blue;
@end
