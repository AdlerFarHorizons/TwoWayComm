<Qucs Schematic 0.0.17>
<Properties>
  <View=0,60,857,800,1,0,0>
  <Grid=10,10,1>
  <DataSet=FeedTest.dat>
  <DataDisplay=FeedTest.dpl>
  <OpenDisplay=1>
  <Script=FeedTest.m>
  <RunScript=0>
  <showFrame=0>
  <FrameText0=Title>
  <FrameText1=Drawn By:>
  <FrameText2=Date:>
  <FrameText3=Revision:>
</Properties>
<Symbol>
</Symbol>
<Components>
  <Pac P1 1 210 300 18 -26 0 1 "1" 1 "35 Ohm" 1 "0 dBm" 0 "1 GHz" 0 "26.85" 0>
  <Pac P2 1 540 290 18 -26 0 1 "2" 1 "73 Ohm" 1 "0 dBm" 0 "1 GHz" 0 "26.85" 0>
  <Pac P3 1 480 350 18 -26 0 1 "3" 1 "73 Ohm" 1 "0 dBm" 0 "1 GHz" 0 "26.85" 0>
  <GND * 1 480 380 0 0 0 0>
  <GND * 1 210 330 0 0 0 0>
  <GND * 1 540 320 0 0 0 0>
  <.SP SP1 1 100 410 0 71 0 0 "lin" 1 "407 MHz" 1 "457 MHz" 1 "11" 1 "no" 0 "1" 0 "2" 0 "no" 0 "no" 0>
  <Sub SUB1 1 380 280 -26 51 0 0 "Feed.sch" 0>
</Components>
<Wires>
  <480 310 480 320 "" 0 0 0 "">
  <410 310 480 310 "" 0 0 0 "">
  <540 250 540 260 "" 0 0 0 "">
  <410 250 540 250 "" 0 0 0 "">
  <210 250 210 270 "" 0 0 0 "">
  <210 250 350 250 "" 0 0 0 "">
  <410 310 410 380 "" 0 0 0 "">
  <330 380 410 380 "" 0 0 0 "">
  <330 310 330 380 "" 0 0 0 "">
  <330 310 350 310 "" 0 0 0 "">
</Wires>
<Diagrams>
  <Smith 610 590 200 200 3 #c0c0c0 1 00 1 0 1 1 1 0 4 1 1 0 1 1 315 0 225 "" "" "">
	<"S[1,1]" #0000ff 0 3 0 5 0>
  </Smith>
</Diagrams>
<Paintings>
</Paintings>
