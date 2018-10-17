<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output indent="yes" encoding="ISO-8859-1"/>

<!-- Helpers -->
<xsl:variable name="colon" select="codepoints-to-string(58)"/>

<!-- Missing parameters -->
<xsl:variable name="leakage" select="1"/>
<xsl:variable name="floor-Uindoors" select="7.5"/>
<xsl:variable name="floor-Usoil" select="4"/>
<xsl:variable name="floor-heatCapacity" select="42000"/>
<xsl:variable name="floor-emissivity" select="0.85"/>
<xsl:variable name="floor-reflectance" select="0.5"/>
<xsl:variable name="rh-max-band" select="10"/>
<xsl:variable name="ventilationThresholdBand" select="1"/>
<xsl:variable name="crackVentilation" select="0.05"/>
<xsl:variable name="crackVentilationTemperatureMin" select="-5"/>
<xsl:variable name="crackVentilationTemperatureMinBand" select="1"/>
<xsl:variable name="screenMaxAtHighRh" select="0.9"/>
<xsl:variable name="screenEnergyThreshold" select="5"/>
<xsl:variable name="screenEnergyThresholdBand" select="5"/>
<xsl:variable name="screenShadeThreshold" select="500"/>
<xsl:variable name="screenShadeThresholdBand" select="50"/>
<xsl:variable name="screenBlackoutFromTime" select="concat('08',$colon,'00',$colon,'00')"/>
<xsl:variable name="screenBlackoutToTime" select="concat('16',$colon,'00',$colon,'00')"/>
<xsl:variable name="heatPipeFlowRate" select="5"/>
<xsl:variable name="heatPipeMaxTemperature" select="60"/>

<!-- Call templates -->
<xsl:template name="float-value">
	<xsl:param name="value"/>
  <xsl:param name="correction" select="0"/>
  <xsl:attribute name="value">
    <xsl:value-of select="number(replace($value, ',', '.')) + $correction"/>
  </xsl:attribute>
</xsl:template>

<xsl:template name="pct-value">
	<xsl:param name="value"/>
  <xsl:attribute name="value">
    <xsl:value-of select="number(replace($value, ',', '.')) div 100"/>
  </xsl:attribute>
</xsl:template>

<xsl:template name="time-value">
	<xsl:param name="value"/>
  <xsl:attribute name="value">
    <xsl:value-of select="concat(substring($value, 1, 2), $colon, substring($value, 3, 2), $colon, '00')"/>
  </xsl:attribute>
</xsl:template>

<xsl:template name="extract-cover">
	<xsl:param name="cover"/>
  <box class="vg::Cover" name="cover">
    <port name="transmissivity">
      <xsl:call-template name="pct-value">
        <xsl:with-param name="value" 
          select="$cover/Constants/Parameters[ParameterName='PaneTransmission']/Value"/>
      </xsl:call-template>
    </port>
    <port name="U4">
      <xsl:call-template name="float-value">
        <xsl:with-param name="value" 
          select="$cover/Constants/Parameters[ParameterName='PaneUValue']/Value"/>
      </xsl:call-template>
    </port>
    </box>
</xsl:template>

<xsl:template name="extract-screen">
  <xsl:if test="Constants/Parameters[ParameterName='ScreenProduct']/Value!='No screen'">
    <xsl:variable name="name" select="lower-case(Constants/Parameters[ParameterName='ScreenTypeText']/Value)"/>
    <box class="vg::Screen" name="MISSING">
      <xsl:attribute name="name">
        <xsl:value-of select="$name"/>
      </xsl:attribute>
      <port name="orientation" value="cover"/>
      <port name="transmissivityLight">
        <xsl:call-template name="pct-value">
          <xsl:with-param name="value" select="Constants/Parameters[ParameterName='Transmission']/Value"/>
        </xsl:call-template>
      </port>
      <port name="energyLossReduction">
        <xsl:call-template name="pct-value">
          <xsl:with-param name="value" select="Constants/Parameters[ParameterName='InsulationRatio']/Value"/>
        </xsl:call-template>
      </port>
      <port name="transmissivityAir">
        <xsl:call-template name="pct-value">
          <xsl:with-param name="value" select="Constants/Parameters[ParameterName='Porosity']/Value"/>
        </xsl:call-template>
      </port>
      <port name="state">
        <xsl:attribute name="ref">
          <xsl:value-of select="concat('actuators/screens/', $name, '[value]')"/>
        </xsl:attribute>
      </port>
    </box>
  </xsl:if>
</xsl:template>

<xsl:template name="extract-screens">
  <xsl:for-each select=".[Layer='1']">
    <xsl:call-template name="extract-screen"/>
  </xsl:for-each>
  <xsl:for-each select=".[Layer='2']">
    <xsl:call-template name="extract-screen"/>
  </xsl:for-each>
  <xsl:for-each select=".[Layer='3']">
    <xsl:call-template name="extract-screen"/>
  </xsl:for-each>
</xsl:template>

<xsl:template name="extract-shelter-face">
	<xsl:param name="position"/>
  <xsl:call-template name="extract-cover">
    <xsl:with-param name="cover" select="DVV_SETUP/Greenhouse/Panes/Pane[Position=$position]"/>
  </xsl:call-template>
  <box class="vg::Screens" name="screens">
    <xsl:for-each select="DVV_SETUP/Greenhouse/Screens/Screen[Position=$position]">
      <xsl:call-template name="extract-screens"/>
    </xsl:for-each>
  </box>
</xsl:template>

<xsl:template name="extract-setpoints">
	<xsl:param name="climateSetpointName"/>
	<xsl:param name="cultureSetpointName"/>
  <xsl:param name="correction" select="0"/>
  <port name="reverseOrder" value="TRUE"/>

  <box name="default">
      <xsl:choose>
        <xsl:when test="string-length(climateSetpointName)=0">
          <newPort name="signal" value="0.0"/>
        </xsl:when>
        <xsl:otherwise>
          <newPort name="signal">
            <xsl:call-template name="float-value">
              <xsl:with-param name="value" 
                select="DVV_SETUP/Greenhouse/zone/Climate/Setpoint/Constants/Parameters[ParameterName=$climateSetpointName]/Value"/>
              <xsl:with-param name="correction" select="$correction"/>
            </xsl:call-template>
          </newPort>
        </xsl:otherwise>
      </xsl:choose>
  </box>

  <xsl:for-each select="DVV_SETUP/Greenhouse/zone/Cultures/Culture/Steps/CultureStep/Setpoints/Setpoint[SetpointName=$cultureSetpointName]/SetpointTimes/SetpointTime">
    <box class="vg::DateTimeSignal">
      <port name="beginDay">
        <xsl:attribute name="value">
          <xsl:value-of select="FromDay"/>
        </xsl:attribute>
      </port>
      <port name="endDay">
        <xsl:attribute name="value">
          <xsl:value-of select="ToDay"/>
        </xsl:attribute>
      </port>
      <port name="beginTime">
        <xsl:call-template name="time-value">
          <xsl:with-param name="value" select="FromTime"/>
        </xsl:call-template>
      </port>
      <port name="endTime">
        <xsl:call-template name="time-value">
          <xsl:with-param name="value" select="ToTime"/>
        </xsl:call-template>
      </port>
      <port name="signalInside">
        <xsl:call-template name="float-value">
          <xsl:with-param name="value" select="SetpointValue"/>
          <xsl:with-param name="correction" select="$correction"/>
        </xsl:call-template>
      </port>
    </box>
  </xsl:for-each>
</xsl:template>

<xsl:template name="setpoint-reference">
  <xsl:param name="setpointName"/>
  <port>
    <xsl:attribute name="name">
      <xsl:value-of select="$setpointName"/>
    </xsl:attribute>
    <xsl:attribute name="ref">
      <xsl:value-of select="concat('allSetpoints/', $setpointName, '[value]')"/>
    </xsl:attribute>
  </port>
</xsl:template>

<xsl:template name="extract-growth-light">
  <xsl:variable name="type" select="number(Constants/Parameters[ParameterName='LampType']/Value)"/>
  <xsl:variable name="intensity" select="number(replace(Constants/Parameters[ParameterName='LightCapacityPerSqm']/Value, ',', '.'))"/>
  <xsl:variable name="parPhotonCoef" select="number(replace(Constants/Parameters[ParameterName='MicromolParPerWatt']/Value, ',', '.'))"/>
  <xsl:if test="$intensity > 0">
    <box class="vg::GrowthLight" name="growthLight">
      <xsl:if test="$type=1">
        <port name="type" value="HPS"/>
        <port name="minPeriodOn" value="30"/>
      </xsl:if>
      <xsl:if test="$type=2">
        <port name="type" value="LED"/>
        <port name="minPeriodOn" value="0"/>
      </xsl:if>
      <port name="intensity">
        <xsl:attribute name="value">
          <xsl:value-of select="$intensity"/>
        </xsl:attribute>
      </port>
      <port name="parPhotonCoef">
        <xsl:attribute name="value">
          <xsl:value-of select="$parPhotonCoef"/>
        </xsl:attribute>
      </port>
      <port name="age">
        <xsl:call-template name="float-value">
          <xsl:with-param name="value" select="Constants/Parameters[ParameterName='UsedHours']/Value"/>
        </xsl:call-template>
      </port>
      <port name="lifeTime">
        <xsl:attribute name="value">
          <xsl:value-of select="Constants/Parameters[ParameterName='LifeTime']/Value"/>
        </xsl:attribute>
      </port>
    </box>
  </xsl:if>
</xsl:template>

<xsl:template name="extract-heat-pipe">
  <xsl:variable name="density" select="number(replace(Constants/Parameters[ParameterName='PipelengthPerSqm']/Value, ',', '.'))"/>
  <xsl:variable name="diameter" select="number(replace(Constants/Parameters[ParameterName='InnerDiameter']/Value, ',', '.'))"/>
  <xsl:variable name="minTemperature" select="number(replace(Constants/Parameters[ParameterName='CommonReturnTemperature']/Value, ',', '.'))"/>
  <xsl:variable name="maxTemperature" select="number(replace(Constants/Parameters[ParameterName='CommonFlowTemperature']/Value, ',', '.'))"/>
  <box class="vg::Pipe" name="pipe">
    <port name="density">
      <xsl:attribute name="value">
        <xsl:value-of select="$density"/>
      </xsl:attribute>
    </port>
    <port name="diameter">
      <xsl:attribute name="value">
        <xsl:value-of select="$diameter * 1000"/>
      </xsl:attribute>
    </port>
    <port name="flowRate">
      <xsl:attribute name="value">
        <xsl:value-of select="$heatPipeFlowRate"/>
      </xsl:attribute>
    </port>
    <port name="minTemperature">
      <xsl:attribute name="value">
        <xsl:value-of select="$minTemperature"/>
      </xsl:attribute>
    </port>
    <port name="maxTemperature">
      <xsl:attribute name="value">
        <xsl:value-of select="$maxTemperature"/>
      </xsl:attribute>
    </port>
  </box>
</xsl:template>

<!-- MAIN -->

<xsl:template match="/"> <box class="Simulation" name="greenhouse">
  <port name="steps" ref="./period[steps]"/>
  <xsl:comment> *** SimulationPeriod *** </xsl:comment>
  <box class="SimulationPeriod" name="period">
    <xsl:variable name="beginDate" select="DVV_SETUP/StartTime"/>
    <xsl:variable name="endDate"  select="DVV_SETUP/StopTime"/>
    <xsl:variable name="beginTime" select="concat('00',$colon,'00',$colon,'00')"/>
    <xsl:variable name="endTime"  select="concat('24',$colon,'00',$colon,'00')"/>
    <port name="beginDate">
      <xsl:attribute name="value">
        <xsl:value-of select="$beginDate"/>
      </xsl:attribute>
    </port>
    <port name="endDate">
      <xsl:attribute name="value">
        <xsl:value-of select="$endDate"/>
      </xsl:attribute>
    </port>
    <port name="beginTime">
      <xsl:attribute name="value">
        <xsl:value-of select="$beginTime"/>
      </xsl:attribute>
    </port>
    <port name="endTime">
      <xsl:attribute name="value">
        <xsl:value-of select="$endTime"/>
      </xsl:attribute>
    </port>
    <port name="timeStep">
      <xsl:call-template name="float-value">
        <xsl:with-param name="value" 
          select="DVV_SETUP/TimeStep"/>
      </xsl:call-template>
    </port>
    <port name="timeUnit" value="m"/>
  </box>
  <xsl:comment> *** Calendar *** </xsl:comment>
  <box class="Calendar" name="calendar">
    <port name="initialDateTime" ref="../period[beginDateTime]"/>
    <port name="timeStep" ref="../period[timeStep]"/>
    <port name="timeUnit" ref="../period[timeUnit]"/>
    <port name="latitude">
      <xsl:call-template name="float-value">
        <xsl:with-param name="value" 
          select="DVV_SETUP/Latitude"/>
      </xsl:call-template>
    </port>
    <port name="longitude">
      <xsl:call-template name="float-value">
        <xsl:with-param name="value" 
          select="DVV_SETUP/Longitude"/>
      </xsl:call-template>
    </port>
  </box>
  <xsl:comment> *** Outdoors *** </xsl:comment>
  <box class="vg::Outdoors" name="outdoors">
  </box>
  <xsl:comment> *** Construction *** </xsl:comment>
  <box name="construction">
    <box class="vg::Geometry" name="geometry">
      <port name="numSpans">
        <xsl:attribute name="value">
          <xsl:value-of select="DVV_SETUP/Greenhouse/Constants/Parameters[ParameterName='NumberOfSpans']/Value"/>
        </xsl:attribute>
      </port>
      <port name="spanWidth">
        <xsl:attribute name="value">
          <xsl:value-of select="DVV_SETUP/Greenhouse/Constants/Parameters[ParameterName='SpanSize']/Value"/>
        </xsl:attribute>
      </port>
      <port name="length">
        <xsl:attribute name="value">
          <xsl:value-of select="DVV_SETUP/Greenhouse/Constants/Parameters[ParameterName='Length']/Value"/>
        </xsl:attribute>
      </port>
      <port name="height">
        <xsl:attribute name="value">
          <xsl:value-of select="DVV_SETUP/Greenhouse/Constants/Parameters[ParameterName='TrempelHeight']/Value"/>
        </xsl:attribute>
      </port>
      <port name="roofPitch">
        <xsl:attribute name="value">
          <xsl:value-of select="DVV_SETUP/Greenhouse/Constants/Parameters[ParameterName='RoofPitch']/Value"/>
        </xsl:attribute>
      </port>
      <port name="reflection">
        <xsl:call-template name="float-value">
          <xsl:with-param name="value" select="DVV_SETUP/Greenhouse/Constants/Parameters[ParameterName='GreenhouseReductionFactorLight']/Value"/>
        </xsl:call-template>
      </port>
    </box>
    <box class="vg::Shelter" name="shelter">
      <box class="vg::ShelterFace" name="roof1">
        <xsl:call-template name="extract-shelter-face">
          <xsl:with-param name="position" select="1"/>
        </xsl:call-template>
      </box>
      <box class="vg::ShelterFace" name="roof2">
        <xsl:call-template name="extract-shelter-face">
          <xsl:with-param name="position" select="2"/>
        </xsl:call-template>
      </box>
      <box class="vg::ShelterFace" name="side1">
        <xsl:call-template name="extract-shelter-face">
          <xsl:with-param name="position" select="3"/>
        </xsl:call-template>
      </box>
      <box class="vg::ShelterFace" name="side2">
        <xsl:call-template name="extract-shelter-face">
          <xsl:with-param name="position" select="4"/>
        </xsl:call-template>
      </box>
      <box class="vg::ShelterFace" name="end1">
        <xsl:call-template name="extract-shelter-face">
          <xsl:with-param name="position" select="5"/>
        </xsl:call-template>
      </box>
      <box class="vg::ShelterFace" name="end2">
        <xsl:call-template name="extract-shelter-face">
          <xsl:with-param name="position" select="6"/>
        </xsl:call-template>
      </box>
    </box>
  </box>
  <xsl:comment> *** Indoors *** </xsl:comment>
  <box class="vg::Indoors" name="indoors">
    <box class="vg::Given" name="given">
      <box class="vg::AirFluxGiven" name="airFlux">
        <box class="vg::AirFluxInfiltration" name="infiltration">
          <port name="leakage">
            <xsl:attribute name="value">
              <xsl:value-of select="$leakage"/>
            </xsl:attribute>
          </port>
        </box>      
      </box>
      <box class="vg::EnergyFluxSum" name="energyFlux">
        <box class="vg::EnergyFluxFloor" name="floor">
          <port name="Uindoors">
            <xsl:attribute name="value">
              <xsl:value-of select="$floor-Uindoors"/>
            </xsl:attribute>
          </port>
          <port name="Usoil">
            <xsl:attribute name="value">
              <xsl:value-of select="$floor-Usoil"/>
            </xsl:attribute>
          </port>
          <port name="heatCapacity">
            <xsl:attribute name="value">
              <xsl:value-of select="$floor-heatCapacity"/>
            </xsl:attribute>
          </port>
          <port name="emissivity">
            <xsl:attribute name="value">
              <xsl:value-of select="$floor-emissivity"/>
            </xsl:attribute>
          </port>
          <box class="vg::FloorRadiationAbsorbed" name="radiationAbsorbed">
            <port name="reflectance">
              <xsl:attribute name="value">
                <xsl:value-of select="$floor-reflectance"/>
              </xsl:attribute>
            </port>
          </box>      
        </box>      
      </box>
    </box>
  </box>
  <xsl:comment> *** allSetpoints *** </xsl:comment>
  <box name="allSetpoints">
    <box class="PrioritySignal" name="heatingTemperatureAtLowRh">
      <xsl:call-template name="extract-setpoints">
        <xsl:with-param name="climateSetpointName" select="'Cli_HeatingTemp'"/>
        <xsl:with-param name="cultureSetpointName" select="'Theat'"/>
      </xsl:call-template>
    </box>
    <box class="PrioritySignal"  name="ventilationTemperatureMargin">
      <xsl:call-template name="extract-setpoints">
        <xsl:with-param name="climateSetpointName" select="'Cli_VentTemp'"/>
        <xsl:with-param name="cultureSetpointName" select="'Tvent'"/>
      </xsl:call-template>
    </box>
    <box class="PrioritySignal"  name="rhMax">
      <xsl:call-template name="extract-setpoints">
        <xsl:with-param name="climateSetpointName" select="'Cli_MaxRelHmd'"/>
        <xsl:with-param name="cultureSetpointName" select="'MaxRH'"/>
        <xsl:with-param name="correction" select="-$rh-max-band"/>
      </xsl:call-template>
    </box>
    <box class="PrioritySignal"  name="heatingTemperatureMargin">
      <xsl:call-template name="extract-setpoints">
        <xsl:with-param name="climateSetpointName" select="'Cli_MaxHeatAddHighRH'"/>
        <xsl:with-param name="cultureSetpointName" select="'MaxTheatRH'"/>
      </xsl:call-template>
    </box>
    <box class="PrioritySignal"  name="ventilationTemperatureRhMargin">
      <xsl:call-template name="extract-setpoints">
        <xsl:with-param name="climateSetpointName" select="'Cli_MaxVentDecHighRH'"/>
        <xsl:with-param name="cultureSetpointName" select="'MaxTventRH'"/>
      </xsl:call-template>
    </box>
    <box class="PrioritySignal" name="co2Min">
      <xsl:call-template name="extract-setpoints">
        <xsl:with-param name="climateSetpointName" select="'Cli_MinCO2'"/>
        <xsl:with-param name="cultureSetpointName" select="'CO2Min'"/>
      </xsl:call-template>
    </box>
    <box class="PrioritySignal" name="co2Max">
      <xsl:call-template name="extract-setpoints">
        <xsl:with-param name="climateSetpointName" select="'Cli_MaxCO2'"/>
        <xsl:with-param name="cultureSetpointName" select="'CO2Max'"/>
      </xsl:call-template>
    </box>
    <box class="PrioritySignal" name="chalk">
      <xsl:call-template name="extract-setpoints">
        <xsl:with-param name="climateSetpointName" select="''"/>
        <xsl:with-param name="cultureSetpointName" select="'ChalkOnOff'"/>
      </xsl:call-template>
    </box>
    <box name="rhMaxBand">
      <newPort name="value">
        <xsl:attribute name="value">
          <xsl:value-of select="$rh-max-band"/>
        </xsl:attribute>
      </newPort>
    </box>
    <box name="dawnThreshold">
      <newPort name="value">
        <xsl:call-template name="float-value">
          <xsl:with-param name="value" 
            select="DVV_SETUP/Greenhouse/Climate/Setpoint/Constants/Parameters[ParameterName='Cli_LightChangeFromNight']/Value"/>
        </xsl:call-template>
      </newPort>
    </box>
    <box name="duskThreshold">
      <newPort name="value">
        <xsl:call-template name="float-value">
          <xsl:with-param name="value" 
            select="DVV_SETUP/Greenhouse/Climate/Setpoint/Constants/Parameters[ParameterName='Cli_LightChangeFromDay']/Value"/>
        </xsl:call-template>
      </newPort>
    </box>
  </box>
  <xsl:comment> *** Setpoints *** </xsl:comment>
  <box class="vg::Setpoints" name="setpoints">
    <xsl:call-template name="setpoint-reference">
      <xsl:with-param name="setpointName" select="'heatingTemperatureAtLowRh'"/>
    </xsl:call-template>
    <xsl:call-template name="setpoint-reference">
      <xsl:with-param name="setpointName" select="'heatingTemperatureMargin'"/>
    </xsl:call-template>
    <xsl:call-template name="setpoint-reference">
      <xsl:with-param name="setpointName" select="'ventilationTemperatureMargin'"/>
    </xsl:call-template>
    <xsl:call-template name="setpoint-reference">
      <xsl:with-param name="setpointName" select="'ventilationTemperatureRhMargin'"/>
    </xsl:call-template>
    <xsl:call-template name="setpoint-reference">
      <xsl:with-param name="setpointName" select="'rhMax'"/>
    </xsl:call-template>
    <xsl:call-template name="setpoint-reference">
      <xsl:with-param name="setpointName" select="'rhMaxBand'"/>
    </xsl:call-template>
    <xsl:call-template name="setpoint-reference">
      <xsl:with-param name="setpointName" select="'co2Min'"/>
    </xsl:call-template>
    <xsl:call-template name="setpoint-reference">
      <xsl:with-param name="setpointName" select="'co2Max'"/>
    </xsl:call-template>
    <xsl:call-template name="setpoint-reference">
      <xsl:with-param name="setpointName" select="'dawnThreshold'"/>
    </xsl:call-template>
    <xsl:call-template name="setpoint-reference">
      <xsl:with-param name="setpointName" select="'duskThreshold'"/>
    </xsl:call-template>
  </box>
  <xsl:comment> *** Controllers *** </xsl:comment>
  <box class="vg::Controllers" name="controllers">
    <port name="ventilationThresholdBand">
      <xsl:attribute name="value">
        <xsl:value-of select="$ventilationThresholdBand"/>
      </xsl:attribute>
    </port>
    <port name="crackVentilation">
      <xsl:attribute name="value">
        <xsl:value-of select="$crackVentilation"/>
      </xsl:attribute>
    </port>
    <port name="crackVentilationTemperatureMin">
      <xsl:attribute name="value">
        <xsl:value-of select="$crackVentilationTemperatureMin"/>
      </xsl:attribute>
    </port>
    <port name="crackVentilationTemperatureMinBand">
      <xsl:attribute name="value">
        <xsl:value-of select="$crackVentilationTemperatureMinBand"/>
      </xsl:attribute>
    </port>
    <port name="screenMaxAtHighRh">
      <xsl:attribute name="value">
        <xsl:value-of select="$screenMaxAtHighRh"/>
      </xsl:attribute>
    </port>
    <port name="screenEnergyThreshold">
      <xsl:attribute name="value">
        <xsl:value-of select="$screenEnergyThreshold"/>
      </xsl:attribute>
    </port>
    <port name="screenEnergyThresholdBand">
      <xsl:attribute name="value">
        <xsl:value-of select="$screenEnergyThresholdBand"/>
      </xsl:attribute>
    </port>
    <port name="screenShadeThreshold">
      <xsl:attribute name="value">
        <xsl:value-of select="$screenShadeThreshold"/>
      </xsl:attribute>
    </port>
    <port name="screenShadeThresholdBand">
      <xsl:attribute name="value">
        <xsl:value-of select="$screenShadeThresholdBand"/>
      </xsl:attribute>
    </port>
    <port name="screenBlackoutFromTime">
      <xsl:attribute name="value">
        <xsl:value-of select="$screenBlackoutFromTime"/>
      </xsl:attribute>
    </port>
    <port name="screenBlackoutToTime">
      <xsl:attribute name="value">
        <xsl:value-of select="$screenBlackoutToTime"/>
      </xsl:attribute>
    </port>
    <box class="vg::Chalk" name="chalk">
      <port name="setpoint" ref="allSetpoints/chalk[value]"/>
    </box>
  </box>

  <xsl:comment> *** Actuators *** </xsl:comment>
  <box class="vg::Actuators" name="actuators">
    <box class="GrowthLights" name="growthLights">
      <xsl:for-each select="DVV_SETUP/Greenhouse/Lamps/Lamp">
        <xsl:call-template name="extract-growth-light"/>
      </xsl:for-each>
    </box>
    <box class="ProportionalSignal" name="heating">
      <port name="signalReset" ref="indoors/temperature[value]"/>
      <port name="input" ref="controllers/heating[value]"/>
      <port name="threshold" value="0"/>
      <port name="thresholdBand" value="1"/>
      <port name="minSignal" ref="indoors/temperature[value]"/>
      <port name="maxSignal">
        <xsl:attribute name="value">
          <xsl:value-of select="$heatPipeMaxTemperature"/>
        </xsl:attribute>
      </port>
      <box name="pipes">
        <xsl:for-each select="DVV_SETUP/Greenhouse/Heatpipes/Heatpipe">
          <xsl:call-template name="extract-heat-pipe"/>
        </xsl:for-each>
      </box>
    </box>
  </box>
       
  <xsl:comment> *** Crop *** </xsl:comment>
  <box class="vg::Crop" name="crop">
    <box name="lai">
      <newPort name="value" value="1"/>
      <newPort name="fractionPlantArea" value="1"/>
    </box>
  </box>
  <xsl:comment> *** Crop *** </xsl:comment>
  <box class="vg::Budget" name="budget"/>
  <xsl:comment> *** Output *** </xsl:comment>
  <box class="OutputR" name="output">
    <box name="p">
      <newPort name="outdoorsCo2" ref="outdoors[co2]"/>
      <newPort name="outdoorsT" ref="outdoors[temperature]"/>
      <newPort name="soilT" ref="outdoors[soilTemperature]"/>
      <newPort name="outdoorsRh" ref="outdoors[rh]"/>
      <newPort name="outdoorsWindSpeed" ref="outdoors[windSpeed]"/>
      <newPort name="outdoorsLight" ref="outdoors[radiation]"/>
      <newPort name="indoorsCo2" ref="indoors/co2[value]"/>
      <newPort name="indoorsT" ref="indoors/temperature[value]"/>
      <newPort name="indoorsRh" ref="indoors/humidity[rh]"/>
      <newPort name="indoorsLight" ref="indoors/light[total]"/>
      <newPort name="growthLightPower" ref="actuators/growthLights[powerUsage] "/>
      <newPort name="heatingPower" ref="controlled/heating/energyFlux[value] "/>
      <newPort name="coolingPower" ref="controlled/cooling/energyFlux[value]"/>
      <newPort name="growthLightIntensity" ref="actuators/growthLights[parIntensity]"/>
      <newPort name="totalLightIntensity" ref="indoors/light[parTotal]"/>
      <newPort name="leafLightUseEfficiency" ref="crop/layers/top/photosynthesis/lightResponse[LUE] "/>
      <newPort name="leafNetPhotosynthesis" ref="crop/growth[netGrowthRate]"/>
    </box>
    <box class="PageR">
      <port name="xAxis" value="calendar[dateTime]"/>
      <box class="PlotR">
        <port name="ncol" value="4"/>
        <port name="ports" value="output/p[*]"/>
      </box>
    </box>
    <box class="OutputText">
      <port name="skipFormats" value="TRUE"/>
      <port name="skipInitialRows" value="720"/>
      <port name="averageN" value="30"/>
    </box>
  </box>
</box> </xsl:template>

</xsl:stylesheet>