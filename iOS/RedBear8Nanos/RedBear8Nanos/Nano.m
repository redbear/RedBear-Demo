//
//  Nano.m
//  RedBear8Nanos
//
//  Created by Sunny Cheung on 27/11/2015.
//  Copyright © 2015 khl. All rights reserved.
//

#import "Nano.h"

@implementation Nano

-(instancetype)initWithDeviceNo:(NSUInteger) deviceNo
{
    self = [super init];
    if (self) {
        _identifier = deviceNo;
        _red = 0;
        _green = 0;
        _blue = 0;
        _status = NO;
    }
    return self;
}

-(NSData *) readStatus
{
//    NSDictionary *dict = [[NSDictionary alloc] initWithObjectsAndKeys:
//                          [[NSNumber alloc] initWithUnsignedInteger:_identifier], @"ID",
//                          [[NSNumber alloc] initWithInt:2], @"opCode",
//                          [[NSNumber alloc] initWithInt:0], @"R",
//                          [[NSNumber alloc] initWithInt:0], @"G",
//                          [[NSNumber alloc] initWithInt:0], @"B",
//                          nil];
//    
//    NSData *data = [NSJSONSerialization dataWithJSONObject:dict options:0 error:nil];
    NSString *sendJson = [NSString stringWithFormat:@"{\"RGB\":{\"ID\":%@,\"OpCode\":%@, \"R\":%@, \"G\":%@, \"B\":%@}}",
                          [[NSNumber alloc] initWithUnsignedInteger:_identifier], [[NSNumber alloc] initWithInt:2],  [NSNumber numberWithInt:_red],  [NSNumber numberWithInt:_green],  [NSNumber numberWithInt:_blue]];
    NSData *data = [sendJson dataUsingEncoding:NSASCIIStringEncoding];

    return data;
}

-(void) updateRed:(NSNumber *) red Green:(NSNumber *) green Blue:(NSNumber *) blue {
    _red = red.intValue;
    _green = green.intValue;
    _blue = blue.intValue;
  
    _status = (_red|| _green|| _blue) ;

    
}
@end
