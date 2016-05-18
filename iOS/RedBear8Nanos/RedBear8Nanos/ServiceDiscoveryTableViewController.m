//
//  ServiceDiscoveryTableViewController.m
//  RedBear8Nanos
//
//  Created by Sunny Cheung on 16/5/2016.
//  Copyright © 2016 khl. All rights reserved.
//

#import "ServiceDiscoveryTableViewController.h"
#import "NanoViewController.h"
#import "SVProgressHUD.h"

@interface ServiceDiscoveryTableViewController ()<NSNetServiceBrowserDelegate, NSNetServiceDelegate, NSStreamDelegate> {
    UIRefreshControl *refreshControl;
}

@property (nonatomic) NSMutableArray *services;
@property (nonatomic) NSNetServiceBrowser *serviceBrowser;
@end

@implementation ServiceDiscoveryTableViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.services = [NSMutableArray arrayWithCapacity:0];
    self.refreshControl = [[UIRefreshControl alloc] init];
    [self.refreshControl addTarget:self action:@selector(searchAgain) forControlEvents:UIControlEventValueChanged];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(sendMsg:) name:@"SETRGB" object:nil];
        // Uncomment the following line to preserve selection between presentations.
    // self.clearsSelectionOnViewWillAppear = NO;
    
    // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
    // self.navigationItem.rightBarButtonItem = self.editButtonItem;
}

-(void) viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    

    
   self.serviceBrowser = [[NSNetServiceBrowser alloc] init];
    
    [self.serviceBrowser setDelegate:self];
    [self.serviceBrowser searchForServicesOfType:@"_duosample._tcp" inDomain:@"local"];

    
}

-(void)viewDidDisappear:(BOOL)animated
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    [super viewDidDisappear:animated];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {

    return [self.services count];
}


- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"RGBServiceCell" forIndexPath:indexPath];
    
    // Configure the cell...
    NSNetService *service = [self.services objectAtIndex:indexPath.row];
    UILabel *hostName = [cell viewWithTag:123];
    [hostName setText:[service hostName]];
    UILabel *address = [cell viewWithTag:789];
    [address setText:[NSString stringWithFormat:@"%@:%ld", [self resolveIPAddress:[service addresses]], [service port]]];
    return cell;
}

-(CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
{
    return 40;
}

-(UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section
{

    UIView *view = [[UIView alloc] initWithFrame:CGRectMake(0, 0, self.tableView.frame.size.width, 40)];
    view.backgroundColor = [UIColor redColor];
    UILabel *serviceLabel = [[UILabel alloc] initWithFrame:CGRectMake(view.frame.origin.x+10, view.frame.origin.y+10, 200, view.frame.size.height-10)];
    [serviceLabel setAttributedText: [[NSAttributedString alloc] initWithString:@"RGB Demo" attributes:@{NSForegroundColorAttributeName:[UIColor whiteColor], NSFontAttributeName:[UIFont fontWithName:@"Helvetica Neue" size:20.0]}]];
    [view addSubview:serviceLabel];
    return view;
}

-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    NSNetService *service = [self.services objectAtIndex:indexPath.row];
    [SVProgressHUD showWithStatus:@"Connecting..."];
    [self initNetworkCommunication:[self resolveIPAddress:[service addresses]] Port:[service port]];
}

#pragma mark - network

- (void) initNetworkCommunication:(NSString *)host Port:(NSInteger) port {

    CFReadStreamRef readStream;
    CFWriteStreamRef writeStream;
    CFStreamCreatePairWithSocketToHost(NULL, (__bridge CFStringRef) host, (UInt32)port, &readStream, &writeStream);
    inputStream = (__bridge NSInputStream *) readStream;
    outputStream = (__bridge NSOutputStream *) writeStream;
    [inputStream setDelegate:self];
    [outputStream setDelegate:self];

    [inputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    [outputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];

    [inputStream open];
    [outputStream open];
}

-(void)stream:(NSStream *)aStream handleEvent:(NSStreamEvent)eventCode
{
    switch (eventCode) {
        case NSStreamEventOpenCompleted:
            [SVProgressHUD dismiss];
            [self performSegueWithIdentifier:@"NanoController" sender:nil];
            break;
        case NSStreamEventHasBytesAvailable:
            if (aStream == inputStream) {
                uint8_t buffer[1024];
                int len;
                
                while ([inputStream hasBytesAvailable]) {
                    len = (int)[inputStream read:buffer maxLength:sizeof(buffer)];
                    
                    if (len > 0) {
                        
                        NSString *output = [[NSString alloc] initWithBytes:buffer length:len encoding:NSASCIIStringEncoding];
                        
                        if (nil != output) {
                            NSLog(@"Got Data: %@", output);
                            [self messageReceived:output];
                        }
                    }
                    
                }
            }
            break;
        case NSStreamEventErrorOccurred:
            [inputStream close];
            [outputStream close];
            [inputStream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
            [outputStream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
            [self disconnect];
            break;
        case NSStreamEventEndEncountered:
            [aStream close];
            [aStream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
            [self disconnect];
            break;
        default: ;
    }
}

-(void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if ([segue.identifier isEqualToString:@"NanoController"]) {
        NanoViewController *vc = segue.destinationViewController;
        vc.outputStream = outputStream;
    }
}

-(void) messageReceived: (NSString *) message
{
    NSLog(@"%@",message);
    NSError *error;
    NSData *data = [message dataUsingEncoding:NSUTF8StringEncoding];
    NSDictionary *json = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableContainers error:&error];
    if (!error) {
        [[NSNotificationCenter defaultCenter] postNotificationName:@"RECEIVE" object:self userInfo:json];
    }
}

-(void) sendMsg:(NSNotification *) notification
{
    
    NSDictionary *json = [notification userInfo];
       NSString *sendJson = [NSString stringWithFormat:@"{\"ID\":%@,\"OpCode\":%@,\"R\":%d,\"G\":%d,\"B\":%d,\"NUM\":0}",
                          json[@"id"],json[@"op"], ((NSNumber *)json[@"r"]).intValue ,((NSNumber *) json[@"g"]).intValue, ((NSNumber *)json[@"b"]).intValue];
    NSLog(@"Send Command: %@", sendJson);
    NSData *data = [sendJson dataUsingEncoding:NSASCIIStringEncoding];
    [outputStream write:[data bytes] maxLength:[data length]];
}


-(void) disconnect
{
    [self.services removeAllObjects];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"DISCONNECTED" object:self];
}



#pragma mark - netServiceBrowserDelegate
-(void)netServiceBrowserWillSearch:(NSNetServiceBrowser *)browser
{

    if (self.refreshControl) {
        [self.refreshControl endRefreshing];
    }
    [SVProgressHUD showWithStatus:@"Searching..."];
}

-(void)netServiceBrowserDidStopSearch:(NSNetServiceBrowser *)browser
{
    if (self.refreshControl) {
        [self.refreshControl endRefreshing];
    }

    [SVProgressHUD dismiss];
    [self.tableView reloadData];
}

-(void)netServiceBrowser:(NSNetServiceBrowser *)browser didNotSearch:(NSDictionary<NSString *,NSNumber *> *)errorDict
{
    if (self.refreshControl) {
        [self.refreshControl endRefreshing];
    }
  
    [SVProgressHUD dismiss];
}


-(void)netServiceBrowser:(NSNetServiceBrowser *)browser didFindService:(NSNetService *)service moreComing:(BOOL)moreComing
{
    service.delegate = self;
    [self.services addObject:service];
    [service resolveWithTimeout:5.0];
}

-(void)netServiceBrowser:(NSNetServiceBrowser *)browser didRemoveService:(NSNetService *)service moreComing:(BOOL)moreComing
{
    [SVProgressHUD dismiss];

    [self.services removeObject:service];

    if (!moreComing) {
        [self.tableView reloadData];
    }
}

#pragma mark - netServiceDelegate
-(void)netServiceDidResolveAddress:(NSNetService *)sender
{
    [SVProgressHUD dismiss];
    
    if ([[sender addresses] count] == 1) {
        [self.tableView reloadData];
    }
    else {
        [SVProgressHUD showInfoWithStatus:@"No Service Found!"];
    }
    
}

-(void)netService:(NSNetService *)sender didNotResolve:(NSDictionary<NSString *,NSNumber *> *)errorDict
{
    [SVProgressHUD showErrorWithStatus:@"An Error occurred.  Please try again"];
    [self.services removeObject:sender];
}

#pragma mark - other
-(NSString *)resolveIPAddress:(NSArray *)addresses
{
    NSData *addressData = [addresses objectAtIndex:0];
    const char *bytes = [addressData bytes];
    return [NSString stringWithFormat:@"%d.%d.%d.%d", (unsigned char)bytes[4],(unsigned char) bytes[5],(unsigned char)bytes[6], (unsigned char)bytes[7]];
    
    
}

-(void) searchAgain
{
     [self.serviceBrowser searchForServicesOfType:@"_duosample._tcp" inDomain:@"local"];
}

@end
