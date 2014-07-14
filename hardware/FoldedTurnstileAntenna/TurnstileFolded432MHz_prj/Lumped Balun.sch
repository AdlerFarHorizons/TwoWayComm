<Qucs Schematic 0.0.17>
<Properties>
  <View=0,10,800,800,1,0,6>
  <Grid=10,10,1>
  <DataSet=test.dat>
  <DataDisplay=test.dpl>
  <OpenDisplay=1>
  <Script=test.m>
  <RunScript=0>
  <showFrame=0>
  <FrameText0=Title>
  <FrameText1=Drawn By:>
  <FrameText2=Date:>
  <FrameText3=Revision:>
</Properties>
<Symbol>
  <.PortSym 40 20 1 0>
  <.PortSym 40 60 2 0>
  <.PortSym 40 100 3 0>
</Symbol>
<Components>
  <L L3 1 180 360 -26 -40 0 2 "1 nH" 1 "" 0>
  <GND * 1 380 320 0 0 0 0>
  <L L1 1 380 220 10 -26 0 1 "1 nH" 1 "" 0>
  <GND * 1 380 540 0 0 0 0>
  <L L4 1 380 510 -56 -26 0 3 "18 nH" 1 "" 0>
  <C C3 1 240 360 -26 -47 0 2 "7.5 pF" 1 "" 0 "neutral" 0>
  <L L2 1 210 70 -26 -40 0 2 "18 nH" 1 "" 0>
  <C C1 1 210 100 -26 17 0 0 "0.1 pF" 1 "" 0 "neutral" 0>
  <C C4 1 410 510 17 -26 0 1 "0.1 pF" 1 "" 0 "neutral" 0>
  <C C2 1 380 280 17 -26 0 1 "7.5 pF" 1 "" 0 "neutral" 0>
  <Port P1 1 60 240 12 4 0 1 "1" 1 "analog" 0>
  <Port P2 1 490 120 12 4 0 1 "2" 1 "analog" 0>
  <Port P3 1 500 380 12 4 0 1 "3" 1 "analog" 0>
</Components>
<Wires>
  <490 100 490 120 "" 0 0 0 "">
  <60 100 180 100 "" 0 0 0 "">
  <60 100 60 210 "" 0 0 0 "">
  <240 70 240 100 "" 0 0 0 "">
  <180 70 180 100 "" 0 0 0 "">
  <240 100 380 100 "" 0 0 0 "">
  <270 360 380 360 "" 0 0 0 "">
  <380 360 380 480 "" 0 0 0 "">
  <380 480 410 480 "" 0 0 0 "">
  <380 540 410 540 "" 0 0 0 "">
  <380 360 500 360 "" 0 0 0 "">
  <500 360 500 380 "" 0 0 0 "">
  <380 310 380 320 "" 0 0 0 "">
  <380 100 490 100 "" 0 0 0 "">
  <380 100 380 190 "" 0 0 0 "">
  <60 210 60 240 "" 0 0 0 "">
  <60 210 150 210 "" 0 0 0 "">
  <150 210 150 360 "" 0 0 0 "">
</Wires>
<Diagrams>
</Diagrams>
<Paintings>
</Paintings>
