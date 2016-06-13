
# RedBear Product Demo

Control the Duo to broadcast as a IBeacon or EddyStone

## Function name:setIBeacon

Set IBeacon UUID (String 16bytes)part,Enable/Disable

Example：{"en":"1","UUID":"64C871727D7E7580B2DED572E11A0000"}

## Function name:setUIDEn

Set UID Enable/Disable,Instance ID(Array 6 bytes)

Example：{"en":"1","iID":["100","120","130","222","225","226"]}

## Function name:setUIDName

Set UID namespace ID (String 10 bytes)

Example：{"nID":"726564626561726C6162"}

## Function name:setURL

Set URL Enable/Disable 

Example：{"en":"1","url":"00726564626561726C616207"}

## Function name:setEID

Set EID(String) Enable/Disable,AES key

Example：{"en":"1","key":"RedBearLab     "}

## Function name:setInterval

Set Advertising Mode transform   Interval

Example：{"int":"5000"}

