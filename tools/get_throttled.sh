#!/bin/bash

#Flag Bits
UNDERVOLTED=0x1
CAPPED=0x2
THROTTLED=0x4
SOFT_TEMPLIMIT=0x8
HAS_UNDERVOLTED=0x10000
HAS_CAPPED=0x20000
HAS_THROTTLED=0x40000
HAS_SOFT_TEMPLIMIT=0x80000


#Text Colors
GREEN=`tput setaf 2`
RED=`tput setaf 1`
NC=`tput sgr0`

#Output Strings
GOOD="${GREEN}NO${NC}"
BAD="${RED}YES${NC}"

#Get Status, extract hex
STATUS=$(vcgencmd get_throttled)
STATUS=${STATUS#*=}

echo -n "Status: "
(($STATUS!=0)) && echo "${RED}${STATUS}${NC}" || echo "${GREEN}${STATUS}${NC}"

echo "Undervolted:"
echo -n "   Now: "
((($STATUS&UNDERVOLTED)!=0)) && echo "${BAD}" || echo "${GOOD}"
echo -n "   Run: "
((($STATUS&HAS_UNDERVOLTED)!=0)) && echo "${BAD}" || echo "${GOOD}"

echo "Throttled:"
echo -n "   Now: "
((($STATUS&THROTTLED)!=0)) && echo "${BAD}" || echo "${GOOD}"
echo -n "   Run: "
((($STATUS&HAS_THROTTLED)!=0)) && echo "${BAD}" || echo "${GOOD}"

echo "Frequency Capped:"
echo -n "   Now: "
((($STATUS&CAPPED)!=0)) && echo "${BAD}" || echo "${GOOD}"
echo -n "   Run: "
((($STATUS&HAS_CAPPED)!=0)) && echo "${BAD}" || echo "${GOOD}"

echo "Softlimit:"
echo -n "   Now: "
((($STATUS&SOFT_TEMPLIMIT)!=0)) && echo "${BAD}" || echo "${GOOD}"
echo -n "   Run: "
((($STATUS&HAS_SOFT_TEMPLIMIT)!=0)) && echo "${BAD}" || echo "${GOOD}"

