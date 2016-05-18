//
//  NanoViewController.m
//  RedBear8Nanos
//
//  Created by Sunny Cheung on 27/11/2015.
//  Copyright © 2015 khl. All rights reserved.
//

#import "NanoViewController.h"
#import "Nano.h"

@interface NanoViewController () {
    NSArray *nanos;
}

@end

@implementation NanoViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    [self initNano];
    [self layoutNanoView];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(receiveMessage:) name:@"RECEIVE" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(disconnect) name:@"DISCONNECTED" object:nil];

}

-(void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    NSString *sendJson = [NSString stringWithFormat:@"{\"ID\":%@,\"OpCode\":%@,\"R\":%@,\"G\":%@,\"B\":%@,\"NUM\":0}",
                          [[NSNumber alloc] initWithUnsignedInteger:240], [[NSNumber alloc] initWithInt:3],  [NSNumber numberWithInt:0],  [NSNumber numberWithInt:0],  [NSNumber numberWithInt:0]];
    NSData *data = [sendJson dataUsingEncoding:NSASCIIStringEncoding];
    [NSTimer scheduledTimerWithTimeInterval:0.2 target:self selector:@selector(sendMessage:) userInfo:@{@"data":data} repeats:NO];
    
}


-(void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:YES];
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}
- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

-(void) initNano
{
    NSMutableArray *array = [[NSMutableArray alloc] init];
    for (int i=0; i < 8; i++) {
        Nano *nano = [[Nano alloc] initWithDeviceNo:i];
        [array addObject:nano];
    }
    nanos = [array copy];
    
}

-(void) layoutNanoView
{
    CGFloat width = [[UIScreen mainScreen] bounds].size.width / 4;
    CGFloat height = [[UIScreen mainScreen] bounds].size.height/ 2;
    NSLog(@"width: %f height: %f", width, height);
    _nano0 = [[[NSBundle mainBundle] loadNibNamed:@"NanoView" owner:nil options:nil] firstObject];
    _nano0.frame = CGRectMake(0, 0, width, height);
    _nano0.nano = [nanos objectAtIndex:0];
   // [_nano0.nano addObserver:self forKeyPath:@"red" options:NSKeyValueObservingOptionNew context:nil];
    
    _nano1 = [[[NSBundle mainBundle] loadNibNamed:@"NanoView" owner:nil options:nil] firstObject];
    _nano1.frame = CGRectMake(width, 0, width, height);
    _nano1.nano = [nanos objectAtIndex:1];
    
    _nano2 = [[[NSBundle mainBundle] loadNibNamed:@"NanoView" owner:nil options:nil] firstObject];
    _nano2.frame = CGRectMake(width * 2, 0, width, height);
    _nano2.nano = [nanos objectAtIndex:2];
    
    _nano3 = [[[NSBundle mainBundle] loadNibNamed:@"NanoView" owner:nil options:nil] firstObject];
    _nano3.frame = CGRectMake(width * 3, 0, width, height);
    _nano3.nano = [nanos objectAtIndex:3];
    
    _nano4 = [[[NSBundle mainBundle] loadNibNamed:@"NanoView" owner:nil options:nil] firstObject];
    _nano4.frame = CGRectMake(0,height, width, height);
    _nano4.nano = [nanos objectAtIndex:4];
    
    _nano5 = [[[NSBundle mainBundle] loadNibNamed:@"NanoView" owner:nil options:nil] firstObject];
    _nano5.frame = CGRectMake(width,height, width, height);
    _nano5.nano = [nanos objectAtIndex:5];
    
    _nano6 = [[[NSBundle mainBundle] loadNibNamed:@"NanoView" owner:nil options:nil] firstObject];
    _nano6.frame = CGRectMake(width*2, height, width, height);
    _nano6.nano = [nanos objectAtIndex:6];
    
    _nano7 = [[[NSBundle mainBundle] loadNibNamed:@"NanoView" owner:nil options:nil] firstObject];
    _nano7.frame = CGRectMake(width * 3, height, width, height);
    _nano7.nano = [nanos objectAtIndex:7];
    

    
    [self.view addSubview:_nano0];
    [self.view addSubview:_nano1];
    [self.view addSubview:_nano2];
    [self.view addSubview:_nano3];
    [self.view addSubview:_nano4];
    [self.view addSubview:_nano5];
    [self.view addSubview:_nano6];
    [self.view addSubview:_nano7];
    

}

-(void) getAllInitStatus: (int) numOfDevice
{
    if (self.outputStream) {
        
        for (int i=0; i < numOfDevice; i++) {
            NSData *data =[[nanos objectAtIndex:i] readStatus];
            [NSTimer scheduledTimerWithTimeInterval:0.2*i target:self selector:@selector(sendMessage:) userInfo:@{@"data":data} repeats:NO];
   
        }
    }
}

-(void) sendMessage:(NSTimer *) timer
{
    NSLog(@"%@",[timer userInfo]);
    NSDictionary *info = [timer userInfo];
    NSData *data = info[@"data"];
    [self.outputStream write:[data bytes] maxLength:[data length]];
}

-(void) receiveMessage: (NSNotification *)notification
{
    NSDictionary *json = [notification userInfo];
    NSLog(@"%@", json);
    NSNumber *deviceNo = json[@"ID"];
    NSNumber *R = json[@"R"];
    NSNumber *G = json[@"G"];
    NSNumber *B = json[@"B"];
    NSNumber *NUM = json[@"NUM"];
    NSNumber *opCode = json[@"OpCode"];
    NSLog(@"%@ %@ %@ %@ %@ %@", deviceNo, R, G, B, opCode, NUM);
    
    if (deviceNo.intValue == 0xf0) {
        [self getAllInitStatus:NUM.intValue];
    }
    else {
        Nano *tmp = nanos[deviceNo.intValue];
        [tmp updateRed:R Green:G Blue:B];
        switch (deviceNo.intValue) {
            case 0: [_nano0 updateView]; break;
         case 1: [_nano1 updateView]; break;
         case 2: [_nano2 updateView]; break;
         case 3: [_nano3 updateView]; break;
         case 4: [_nano4 updateView]; break;
         case 5: [_nano5 updateView]; break;
         case 6: [_nano6 updateView]; break;
         case 7: [_nano7 updateView]; break;
        }
    }
}

-(void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary<NSString *,id> *)change context:(void *)context
{
    NSLog(@"%@", object);
}

-(void) disconnect
{
    NSLog(@"Disconnected");
  
    [self dismissViewControllerAnimated:YES completion:nil];
    
}
@end
