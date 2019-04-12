<xsl:stylesheet version="2.0" 
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:ecolmod="http://www.ecolmod.org">
<xsl:output indent="yes" encoding="ISO-8859-1"/>

<!-- Helpers -->
<xsl:variable name="colon" select="codepoints-to-string(58)"/>
<xsl:variable name="test-weather-file" select="'input/sel_dk.txt'"/> 

<!-- Missing parameters -->
<xsl:variable name="heatPipeMaxTemperature" select="60"/>
<xsl:variable name="HPS-minPeriodOn" select="30"/>
<xsl:variable name="LED-minPeriodOn" select="0"/>

<!-- Call templates -->
<xsl:template name="float-with-period">
  <xsl:param name="value"/>
  <xsl:choose>
    <xsl:when test="contains(string($value), '.')">
      <xsl:attribute name="value">
        <xsl:value-of select="$value"/>
      </xsl:attribute>
    </xsl:when>
    <xsl:otherwise>
      <xsl:attribute name="value">
        <xsl:value-of select="concat($value, '.0')"/>
      </xsl:attribute>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template name="float-value">
  <xsl:param name="value"/>
  <xsl:param name="correction" select="0"/>
  <xsl:call-template name="float-with-period">
    <xsl:with-param name="value" select="number(replace($value, ',', '.')) + $correction"/>
  </xsl:call-template>
</xsl:template>

<xsl:template name="pct-value">
  <xsl:param name="value"/>
  <xsl:call-template name="float-with-period">
    <xsl:with-param name="value" select="number(replace($value, ',', '.')) div 100"/>
  </xsl:call-template>
</xsl:template>

<xsl:template name="time-value">
  <xsl:param name="value"/>
  <xsl:attribute name="value">
    <xsl:value-of select="concat(substring($value, 1, 2), $colon, substring($value, 3, 2), $colon, '00')"/>
  </xsl:attribute>
</xsl:template>

<xsl:function name="ecolmod:generateXPath" >
  <xsl:param name="pNode" as="node()"/>

  <xsl:for-each select="$pNode/ancestor::*">    
    <xsl:value-of select="concat('/', name())" />    
  </xsl:for-each>    
  <xsl:value-of select="concat('/', name($pNode))" />     
</xsl:function>

<xsl:template name="extract-cover">
  <xsl:param name="cover"/>

  <xsl:variable name="transmissivityName" select="'PaneTransmission'"/>
  <xsl:variable name="transmissivitySrc" select="$cover/Constants/Parameters[ParameterName=$transmissivityName]/Value" as="node()"/>
  <xsl:variable name="U4Name" select="'PaneUValue'"/>
  <xsl:variable name="U4Src" select="$cover/Constants/Parameters[ParameterName=$U4Name]/Value" as="node()"/>

  <box class="vg::Cover" name="cover">
    <port name="transmissivity">
      <xsl:attribute name="externalName">
        <xsl:value-of select="$transmissivityName"/>
      </xsl:attribute>
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($transmissivitySrc)"/>
      </xsl:attribute>
      <xsl:call-template name="pct-value">
        <xsl:with-param name="value" select="$transmissivitySrc"/>
      </xsl:call-template>
    </port>

    <port name="U4">
      <xsl:attribute name="externalName">
        <xsl:value-of select="$U4Name"/>
      </xsl:attribute>
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($U4Src)"/>
      </xsl:attribute>
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
    
    <xsl:variable name="transmissivityLightName" select="'Transmission'"/>
    <xsl:variable name="transmissivityLightSrc" select="Constants/Parameters[ParameterName=$transmissivityLightName]/Value" as="node()"/>
    <xsl:variable name="energyLossReductionName" select="'InsulationRatio'"/>
    <xsl:variable name="energyLossReductionSrc" select="Constants/Parameters[ParameterName=$energyLossReductionName]/Value" as="node()"/>
    <xsl:variable name="transmissivityAirName" select="'Porosity'"/>
    <xsl:variable name="transmissivityAirSrc" select="Constants/Parameters[ParameterName=$transmissivityAirName]/Value" as="node()"/>
    
        <box class="vg::Screen" name="MISSING">
      <xsl:attribute name="name">
        <xsl:value-of select="$name"/>
      </xsl:attribute>
      <port name="orientation" externalName="No name" source="Fixed" value="cover"/>
      <port name="transmissivityLight">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$transmissivityLightName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($transmissivityLightSrc)"/>
        </xsl:attribute>
        <xsl:call-template name="pct-value">
          <xsl:with-param name="value" select="$transmissivityLightSrc"/>
        </xsl:call-template>
      </port>
      <port name="energyLossReduction">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$energyLossReductionName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($energyLossReductionSrc)"/>
        </xsl:attribute>
        <xsl:call-template name="pct-value">
          <xsl:with-param name="value" select="$energyLossReductionSrc"/>
        </xsl:call-template>
      </port>
      <port name="transmissivityAir">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$transmissivityAirName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($transmissivityAirSrc)"/>
        </xsl:attribute>
        <xsl:call-template name="pct-value">
          <xsl:with-param name="value" select="$transmissivityAirSrc"/>
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

<xsl:template name="extract-vent">
  <xsl:param name="position"/>
  <xsl:variable name="lengthName" select="'Length'"/>
  <xsl:variable name="lengthSrc" select="DVV_SETUP/Greenhouse/Vents/Vent[Position=$position]/Constants/Parameters[ParameterName=$lengthName]/Value"/>
  <xsl:variable name="lengthValue" select="number(replace($lengthSrc, ',', '.'))"/>
  <xsl:variable name="widthName" select="'Width'"/>
  <xsl:variable name="widthSrc" select="DVV_SETUP/Greenhouse/Vents/Vent[Position=$position]/Constants/Parameters[ParameterName=$widthName]/Value"/>
  <xsl:variable name="widthValue" select="number(replace($widthSrc, ',', '.'))"/>
  <xsl:variable name="numberName" select="'Number'"/>
  <xsl:variable name="numberSrc" select="DVV_SETUP/Greenhouse/Vents/Vent[Position=$position]/Constants/Parameters[ParameterName=$numberName]/Value"/>
  <xsl:variable name="numberValue" select="number(replace($numberSrc, ',', '.'))"/>
  <xsl:variable name="transmissivityName" select="'VentTransmission'"/>
  <xsl:variable name="transmissivitySrc" select="DVV_SETUP/Greenhouse/Vents/Vent[Position=$position]/Constants/Parameters[ParameterName=$transmissivityName]/Value"/>
  <xsl:variable name="transmissivityValue" select="number(replace($transmissivitySrc, ',', '.'))"/>

  <box class="vg::Vent" name="vent">
    <port name="length">
      <xsl:attribute name="externalName">
        <xsl:value-of select="$lengthName"/>
      </xsl:attribute>
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($lengthSrc)"/>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:value-of select="$lengthValue"/>
      </xsl:attribute>
    </port>
    <port name="width">
      <xsl:attribute name="externalName">
        <xsl:value-of select="$widthName"/>
      </xsl:attribute>
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($widthSrc)"/>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:value-of select="$widthValue"/>
      </xsl:attribute>
    </port>
    <port name="number">
      <xsl:attribute name="externalName">
        <xsl:value-of select="$numberName"/>
      </xsl:attribute>
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($numberSrc)"/>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:value-of select="$numberValue"/>
      </xsl:attribute>
    </port>
    <port name="transmissivity">
      <xsl:attribute name="externalName">
        <xsl:value-of select="$transmissivityName"/>
      </xsl:attribute>
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($transmissivitySrc)"/>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:value-of select="$transmissivityValue"/>
      </xsl:attribute>
    </port>
  </box>
</xsl:template>

<xsl:template name="extract-setpoints">
  <xsl:param name="climateSetpointName"/>
  <xsl:param name="correction" select="0"/>

  <port name="reverseOrder" value="TRUE"/>

  <box name="default">
      <xsl:choose>
        <xsl:when test="string-length($climateSetpointName)=0">
          <newPort name="signal" value="0.0"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:variable name="signalSrc" 
                        select="DVV_SETUP/Greenhouse/Climate/Setpoint/Constants/Parameters[ParameterName=$climateSetpointName]/Value"/>
          <newPort name="signal">
            <xsl:attribute name="externalName">
              <xsl:value-of select="$climateSetpointName"/>
            </xsl:attribute>
            <xsl:if test="string-length($signalSrc)=0">
              <xsl:attribute name="source">
                <xsl:value-of select="'Missing'"/>
              </xsl:attribute>
            </xsl:if>
            <xsl:if test="string-length($signalSrc)!=0">
              <xsl:attribute name="source">
                <xsl:value-of select="ecolmod:generateXPath($signalSrc)"/>
              </xsl:attribute>
            </xsl:if>
            <xsl:call-template name="float-value">
              <xsl:with-param name="value" select="$signalSrc"/>
              <!-- <xsl:with-param name="correction" select="$correction"/> -->
            </xsl:call-template>
          </newPort>
        </xsl:otherwise>
      </xsl:choose>
  </box>

  <xsl:variable name="setpointSrc" 
                select="DVV_SETUP/Greenhouse/Climate/Setpoint/Constants[Parameters/ParameterName=$climateSetpointName]/ModSetpoint"/>

  <xsl:for-each select="$setpointSrc">
    <box class="vg::DateTimeSignal">
      <port name="beginDate">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$climateSetpointName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($setpointSrc/FromDay)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="FromDay"/>
        </xsl:attribute>
      </port>
      <port name="endDate">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$climateSetpointName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($setpointSrc/ToDay)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="ToDay"/>
        </xsl:attribute>
      </port>
      <port name="beginTime">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$climateSetpointName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($setpointSrc/FromTime)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="FromTime"/>
        </xsl:attribute>
      </port>
      <port name="endTime">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$climateSetpointName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($setpointSrc/ToTime)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="ToTime"/>
        </xsl:attribute>
      </port>
      <port name="signalInside">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$climateSetpointName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($setpointSrc/Value)"/>
        </xsl:attribute>
        <xsl:call-template name="float-value">
          <xsl:with-param name="value" select="Value"/>
          <!-- <xsl:with-param name="correction" select="$correction"/> -->
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
  <xsl:variable name="typeName" select="'LampType'"/>
  <xsl:variable name="typeSrc" select="Constants/Parameters[ParameterName=$typeName]/Value"/>
  <xsl:variable name="intensityName" select="'LightCapacityPerSqm'"/>
  <xsl:variable name="intensitySrc" select="Constants/Parameters[ParameterName=$intensityName]/Value"/>
  <xsl:variable name="intensityValue" select="number(replace($intensitySrc, ',', '.'))"/>
  <xsl:variable name="parPhotonCoefName" select="'MicromolParPerWatt'"/>
  <xsl:variable name="parPhotonCoefSrc" select="Constants/Parameters[ParameterName=$parPhotonCoefName]/Value"/>
  <xsl:variable name="parPhotonCoefValue" select="number(replace($parPhotonCoefSrc, ',', '.'))"/>
  <xsl:variable name="ageName" select="'UsedHours'"/>
  <xsl:variable name="ageSrc" select="Constants/Parameters[ParameterName=$ageName]/Value"/>
  <xsl:variable name="ageValue" select="number(replace($ageSrc, ',', '.'))"/>
  <xsl:variable name="lifeTimeName" select="'LifeTime'"/>
  <xsl:variable name="lifeTimeSrc" select="Constants/Parameters[ParameterName=$lifeTimeName]/Value"/>
  <xsl:variable name="lifeTimeValue" select="number(replace($lifeTimeSrc, ',', '.'))"/>

  <xsl:if test="$intensityValue > 0">
    <box class="vg::GrowthLight" name="growthLight">
      <xsl:if test="number($typeSrc)=1">
        <port name="type" value="HPS">
          <xsl:attribute name="externalName">
            <xsl:value-of select="$typeName"/>
          </xsl:attribute>
          <xsl:attribute name="source">
            <xsl:value-of select="ecolmod:generateXPath($typeSrc)"/>
          </xsl:attribute>
        </port>
        <port name="minPeriodOn" externalName="HPS-minPeriodOn" source="Fixed">
          <xsl:attribute name="value">
            <xsl:value-of select="$HPS-minPeriodOn"/>
          </xsl:attribute>
        </port>
      </xsl:if>
      <xsl:if test="number($typeSrc)=2">
        <port name="type" value="LED">
          <xsl:attribute name="externalName">
            <xsl:value-of select="$typeName"/>
          </xsl:attribute>
          <xsl:attribute name="source">
            <xsl:value-of select="ecolmod:generateXPath($typeSrc)"/>
          </xsl:attribute>
        </port>
        <port name="minPeriodOn" externalName="LED-minPeriodOn" source="Fixed">
          <xsl:attribute name="value">
            <xsl:value-of select="$LED-minPeriodOn"/>
          </xsl:attribute>
        </port>
      </xsl:if>
      
      <port name="intensity">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$intensityName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($intensitySrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$intensityValue"/>
        </xsl:attribute>
      </port>
      <port name="parPhotonCoef">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$parPhotonCoefName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($parPhotonCoefSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$parPhotonCoefValue"/>
        </xsl:attribute>
      </port>
      <port name="age">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$ageName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($ageSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$ageValue"/>
        </xsl:attribute>
      </port>
      <port name="lifeTime">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$lifeTimeName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($lifeTimeSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$lifeTimeValue"/>
        </xsl:attribute>
      </port>
    </box>
  </xsl:if>
</xsl:template>

<xsl:template name="extract-heat-pipe">
  <xsl:variable name="materialName" select="'PipeMaterialDesc'"/>
  <xsl:variable name="materialSrc" select="Constants/Parameters[ParameterName=$materialName]/Value" as="node()"/>
  <xsl:variable name="materialValue" select="$materialSrc"/>
  <xsl:variable name="densityName" select="'PipeLengthPerSqm'"/>
  <xsl:variable name="densitySrc" select="Constants/Parameters[ParameterName=$densityName]/Value" as="node()"/>
  <xsl:variable name="densityValue" select="number(replace($densitySrc, ',', '.'))"/>
  <xsl:variable name="diameterName" select="'InnerDiameter'"/>
  <xsl:variable name="diameterSrc" select="Constants/Parameters[ParameterName=$diameterName]/Value" as="node()"/>
  <xsl:variable name="diameterValue" select="number(replace($diameterSrc, ',', '.'))"/>
  <xsl:variable name="minTemperatureName" select="'CommonReturnTemperature'"/>
  <xsl:variable name="minTemperatureSrc" select="Constants/Parameters[ParameterName=$minTemperatureName]/Value" as="node()"/>
  <xsl:variable name="minTemperatureValue" select="number(replace($minTemperatureSrc, ',', '.'))"/>
  <xsl:variable name="maxTemperatureName" select="'CommonFlowTemperature'"/>
  <xsl:variable name="maxTemperatureSrc" select="Constants/Parameters[ParameterName=$maxTemperatureName]/Value" as="node()"/>
  <xsl:variable name="maxTemperatureValue" select="number(replace($maxTemperatureSrc, ',', '.'))"/>
  <xsl:variable name="flowRateName" select="'heatPipeFlowRate'"/>
  <xsl:variable name="flowRateSrc" select="Constants/Parameters[ParameterName=$flowRateName]/Value" as="node()"/>
  <xsl:variable name="flowRateValue" select="number(replace($flowRateSrc, ',', '.'))"/>

  <box class="vg::Pipe" name="pipe">
    <port name="density">
      <xsl:attribute name="externalName">
        <xsl:value-of select="$densityName"/>
      </xsl:attribute>
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($densitySrc)"/>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:value-of select="$densityValue"/>
      </xsl:attribute>
    </port>
    <port name="diameter">
      <xsl:attribute name="externalName">
        <xsl:value-of select="$diameterName"/>
      </xsl:attribute>
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($diameterSrc)"/>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:value-of select="$diameterValue * 1000"/>
      </xsl:attribute>
    </port>
    <port name="flowRate">
      <xsl:attribute name="externalName">
        <xsl:value-of select="$flowRateName"/>
      </xsl:attribute>
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($flowRateSrc)"/>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:value-of select="$flowRateValue"/>
      </xsl:attribute>
    </port>
    <port name="minTemperature">
      <xsl:attribute name="externalName">
        <xsl:value-of select="$minTemperatureName"/>
      </xsl:attribute>
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($minTemperatureSrc)"/>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:value-of select="$minTemperatureValue"/>
      </xsl:attribute>
    </port>
    <port name="maxTemperature">
      <xsl:attribute name="externalName">
        <xsl:value-of select="$maxTemperatureName"/>
      </xsl:attribute>
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($maxTemperatureSrc)"/>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:value-of select="$maxTemperatureValue"/>
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
    <xsl:variable name="endDate"   select="DVV_SETUP/StopTime"/>
    <xsl:variable name="beginTime" select="concat('00',$colon,'00',$colon,'00')"/>
    <xsl:variable name="endTime"   select="concat('00',$colon,'00',$colon,'00')"/>
    <xsl:variable name="timeStep"  select="DVV_SETUP/TimeStep"/>
    <port name="beginDate" externalName="None">
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($beginDate)"/>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:value-of select="$beginDate"/>
      </xsl:attribute>
    </port>
    <port name="endDate" externalName="None">
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($endDate)"/>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:value-of select="$endDate"/>
      </xsl:attribute>
    </port>
    <port name="beginTime" externalName="None" source="Fixed">
      <xsl:attribute name="value">
        <xsl:value-of select="$beginTime"/>
      </xsl:attribute>
    </port>
    <port name="endTime" externalName="None" source="Fixed">
      <xsl:attribute name="value">
        <xsl:value-of select="$endTime"/>
      </xsl:attribute>
    </port>
    <port name="timeStep" externalName="None">
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($timeStep)"/>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:value-of select="$timeStep"/>
      </xsl:attribute>
    </port>
    <port name="timeUnit" value="m" externalName="None" source="Fixed"/>
  </box>
  <xsl:comment> *** Calendar *** </xsl:comment>
  <xsl:variable name="latitude"  select="DVV_SETUP/Latitude"/>
  <xsl:variable name="longitude" select="DVV_SETUP/Longitude"/>
  <box class="Calendar" name="calendar">
    <port name="initialDateTime" ref="../period[beginDateTime]"/>
    <port name="timeStep" ref="../period[timeStep]"/>
    <port name="timeUnit" ref="../period[timeUnit]"/>
    <port name="latitude" externalName="None">
      <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($latitude)"/>
        </xsl:attribute>
        <xsl:call-template name="float-value">
          <xsl:with-param name="value" select="$latitude"/>
        </xsl:call-template>
    </port>
    <port name="longitude" externalName="None">
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($longitude)"/>
      </xsl:attribute>
      <xsl:call-template name="float-value">
        <xsl:with-param name="value" select="$longitude"/>
      </xsl:call-template>
    </port>
  </box>
  <xsl:comment> *** Outdoors *** </xsl:comment>
  <xsl:variable name="fileName"  select="DVV_SETUP/Weather/File"/>
  <box class="vg::Outdoors" name="outdoors">
    <box class="Records" name="records">
      <port name="fileName" externalName="None">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($fileName)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$fileName"/>
          <!-- <xsl:value-of select="$test-weather-file"/> -->
        </xsl:attribute>
      </port>
      <port name="ignoreYear" externalName="None" source="Fixed" value="TRUE"/>
    </box>
  </box>
  <xsl:comment> *** Construction *** </xsl:comment>
  <xsl:variable name="orientationName" select="'Orientation'"/>
  <xsl:variable name="orientationSrc" select="DVV_SETUP/Greenhouse/Constants/Parameters[ParameterName=$orientationName]/Value" as="node()"/>
  <xsl:variable name="orientationValue" select="number(replace($orientationSrc, ',', '.'))"/>
  <xsl:variable name="numSpansName" select="'NumberOfSpans'"/>
  <xsl:variable name="numSpansSrc" select="DVV_SETUP/Greenhouse/Constants/Parameters[ParameterName=$numSpansName]/Value" as="node()"/>
  <xsl:variable name="numSpansValue" select="number(replace($numSpansSrc, ',', '.'))"/>
  <xsl:variable name="spanWidthName" select="'SpanSize'"/>
  <xsl:variable name="spanWidthSrc" select="DVV_SETUP/Greenhouse/Constants/Parameters[ParameterName=$spanWidthName]/Value" as="node()"/>
  <xsl:variable name="spanWidthValue" select="number(replace($spanWidthSrc, ',', '.'))"/>
  <xsl:variable name="lengthName" select="'Length'"/>
  <xsl:variable name="lengthSrc" select="DVV_SETUP/Greenhouse/Constants/Parameters[ParameterName=$lengthName]/Value" as="node()"/>
  <xsl:variable name="lengthValue" select="number(replace($lengthSrc, ',', '.'))"/>
  <xsl:variable name="heightName" select="'TrempelHeight'"/>
  <xsl:variable name="heightSrc" select="DVV_SETUP/Greenhouse/Constants/Parameters[ParameterName=$heightName]/Value" as="node()"/>
  <xsl:variable name="heightValue" select="number(replace($heightSrc, ',', '.'))"/>
  <xsl:variable name="roofPitchName" select="'RoofPitch'"/>
  <xsl:variable name="roofPitchSrc" select="DVV_SETUP/Greenhouse/Constants/Parameters[ParameterName=$roofPitchName]/Value" as="node()"/>
  <xsl:variable name="roofPitchValue" select="number(replace($roofPitchSrc, ',', '.'))"/>
  <xsl:variable name="reflectionName" select="'GreenhouseReductionFactorLight'"/>
  <xsl:variable name="reflectionSrc" select="DVV_SETUP/Greenhouse/Constants/Parameters[ParameterName=$reflectionName]/Value" as="node()"/>
  <xsl:variable name="reflectionValue" select="number(replace($reflectionSrc, ',', '.'))"/>

  <box name="construction">
    <box class="vg::Geometry" name="geometry">
      <port name="orientation">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$orientationName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($orientationSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$orientationValue"/>
        </xsl:attribute>
      </port>
      <port name="numSpans">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$numSpansName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($numSpansSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$numSpansValue"/>
        </xsl:attribute>
      </port>
      <port name="spanWidth">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$spanWidthName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($spanWidthSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$spanWidthValue"/>
        </xsl:attribute>
      </port>
      <port name="length">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$lengthName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($lengthSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$lengthValue"/>
        </xsl:attribute>
      </port>
      <port name="height">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$heightName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($heightSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$heightValue"/>
        </xsl:attribute>
      </port>
      <port name="roofPitch">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$roofPitchName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($roofPitchSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$roofPitchValue"/>
        </xsl:attribute>
      </port>
      <port name="reflection">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$reflectionName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($reflectionSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$reflectionValue"/>
        </xsl:attribute>
      </port>
    </box>
    <box class="vg::Shelter" name="shelter">
      <box class="vg::ShelterFace" name="roof1">
        <xsl:call-template name="extract-shelter-face">
          <xsl:with-param name="position" select="1"/>
        </xsl:call-template>
        <!-- <xsl:call-template name="extract-vent"> -->
          <!-- <xsl:with-param name="position" select="1"/> -->
        <!-- </xsl:call-template> -->
      </box>
      <box class="vg::ShelterFace" name="roof2">
        <xsl:call-template name="extract-shelter-face">
          <xsl:with-param name="position" select="2"/>
        </xsl:call-template>
        <!-- <xsl:call-template name="extract-vent"> -->
          <!-- <xsl:with-param name="position" select="2"/> -->
        <!-- </xsl:call-template> -->
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
  <xsl:variable name="leakageName" select="'leakage'"/>
  <xsl:variable name="leakageSrc" select="DVV_SETUP/Greenhouse/Constants/Parameters[ParameterName=$leakageName]/Value" as="node()"/>
  <xsl:variable name="leakageValue" select="number(replace($leakageSrc, ',', '.'))"/>
  <xsl:variable name="floorUindoorsName" select="'floor-Uindoors'"/>
  <xsl:variable name="floorUindoorsSrc" select="DVV_SETUP/Greenhouse/Constants/Parameters[ParameterName=$floorUindoorsName]/Value" as="node()"/>
  <xsl:variable name="floorUindoorsValue" select="number(replace($floorUindoorsSrc, ',', '.'))"/>
  <xsl:variable name="floorUsoilName" select="'floor-Usoil'"/>
  <xsl:variable name="floorUsoilSrc" select="DVV_SETUP/Greenhouse/Constants/Parameters[ParameterName=$floorUsoilName]/Value" as="node()"/>
  <xsl:variable name="floorUsoilValue" select="number(replace($floorUsoilSrc, ',', '.'))"/>
  <xsl:variable name="floorHeatCapacityName" select="'floor-heatCapacity'"/>
  <xsl:variable name="floorHeatCapacitySrc" select="DVV_SETUP/Greenhouse/Constants/Parameters[ParameterName=$floorHeatCapacityName]/Value" as="node()"/>
  <xsl:variable name="floorHeatCapacityValue" select="number(replace($floorHeatCapacitySrc, ',', '.'))"/>
  <xsl:variable name="floorEmissivityName" select="'floor-emissivity'"/>
  <xsl:variable name="floorEmissivitySrc" select="DVV_SETUP/Greenhouse/Constants/Parameters[ParameterName=$floorEmissivityName]/Value" as="node()"/>
  <xsl:variable name="floorEmissivityValue" select="number(replace($floorEmissivitySrc, ',', '.'))"/>
  <xsl:variable name="floorReflectanceName" select="'floor-reflectance'"/>
  <xsl:variable name="floorReflectanceSrc" select="DVV_SETUP/Greenhouse/Constants/Parameters[ParameterName=$floorReflectanceName]/Value" as="node()"/>
  <xsl:variable name="floorReflectanceValue" select="number(replace($floorReflectanceSrc, ',', '.'))"/>
  <box class="vg::Indoors" name="indoors">
    <box class="vg::Given" name="given">
      <box class="vg::AirFluxGiven" name="airFlux">
        <box class="vg::AirFluxInfiltration" name="infiltration">
          <port name="leakage">
            <xsl:attribute name="externalName">
              <xsl:value-of select="$leakageName"/>
            </xsl:attribute>
            <xsl:attribute name="source">
              <xsl:value-of select="ecolmod:generateXPath($leakageSrc)"/>
            </xsl:attribute>
            <xsl:attribute name="value">
              <xsl:value-of select="$leakageValue"/>
            </xsl:attribute>
          </port>
        </box>      
      </box>
      <box class="vg::EnergyFluxSum" name="energyFlux">
        <box class="vg::EnergyFluxFloor" name="floor">
          <port name="Uindoors">
            <xsl:attribute name="externalName">
              <xsl:value-of select="$floorUindoorsName"/>
            </xsl:attribute>
            <xsl:attribute name="source">
              <xsl:value-of select="ecolmod:generateXPath($floorUindoorsSrc)"/>
            </xsl:attribute>
            <xsl:attribute name="value">
              <xsl:value-of select="$floorUindoorsValue"/>
            </xsl:attribute>
          </port>
          <port name="Usoil">
            <xsl:attribute name="externalName">
              <xsl:value-of select="$floorUsoilName"/>
            </xsl:attribute>
            <xsl:attribute name="source">
              <xsl:value-of select="ecolmod:generateXPath($floorUsoilSrc)"/>
            </xsl:attribute>
            <xsl:attribute name="value">
              <xsl:value-of select="$floorUsoilValue"/>
            </xsl:attribute>
          </port>
          <port name="heatCapacity">
            <xsl:attribute name="externalName">
              <xsl:value-of select="$floorHeatCapacityName"/>
            </xsl:attribute>
            <xsl:attribute name="source">
              <xsl:value-of select="ecolmod:generateXPath($floorHeatCapacitySrc)"/>
            </xsl:attribute>
            <xsl:attribute name="value">
              <xsl:value-of select="$floorHeatCapacityValue"/>
            </xsl:attribute>
          </port>
          <port name="emissivity">
            <xsl:attribute name="externalName">
              <xsl:value-of select="$floorEmissivityName"/>
            </xsl:attribute>
            <xsl:attribute name="source">
              <xsl:value-of select="ecolmod:generateXPath($floorEmissivitySrc)"/>
            </xsl:attribute>
            <xsl:attribute name="value">
              <xsl:value-of select="$floorEmissivityValue"/>
            </xsl:attribute>
          </port>
          <box class="vg::FloorRadiationAbsorbed" name="radiationAbsorbed">
            <port name="reflectance">
              <xsl:attribute name="externalName">
                <xsl:value-of select="$floorReflectanceName"/>
              </xsl:attribute>
              <xsl:attribute name="source">
                <xsl:value-of select="ecolmod:generateXPath($floorReflectanceSrc)"/>
              </xsl:attribute>
              <xsl:attribute name="value">
                <xsl:value-of select="$floorReflectanceValue"/>
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
      </xsl:call-template>
    </box>
    <box class="PrioritySignal"  name="ventilationTemperatureMargin">
      <xsl:call-template name="extract-setpoints">
        <xsl:with-param name="climateSetpointName" select="'Cli_VentTemp'"/>
      </xsl:call-template>
    </box>
    <box class="PrioritySignal"  name="rhMax">
      <xsl:call-template name="extract-setpoints">
        <xsl:with-param name="climateSetpointName" select="'Cli_MaxRelHmd'"/>
      </xsl:call-template>
    </box>
    <box class="PrioritySignal"  name="heatingTemperatureMargin">
      <xsl:call-template name="extract-setpoints">
        <xsl:with-param name="climateSetpointName" select="'Cli_MaxHeatAddHighRH'"/>
      </xsl:call-template>
    </box>
   <box class="PrioritySignal"  name="ventilationTemperatureRhMargin">
      <xsl:call-template name="extract-setpoints">
        <xsl:with-param name="climateSetpointName" select="'Cli_MaxVentDecHighRH'"/>
      </xsl:call-template>
    </box>
   <box class="PrioritySignal"  name="ventilationThresholdBand">
      <xsl:call-template name="extract-setpoints">
        <xsl:with-param name="climateSetpointName" select="'ventilationsThresholdBand'"/>
      </xsl:call-template>
    </box>
    <box class="PrioritySignal" name="co2Min">
      <xsl:call-template name="extract-setpoints">
        <xsl:with-param name="climateSetpointName" select="'Cli_MinCO2'"/>
      </xsl:call-template>
    </box>
    <box class="PrioritySignal" name="co2Max">
      <xsl:call-template name="extract-setpoints">
        <xsl:with-param name="climateSetpointName" select="'Cli_MaxCO2'"/>
      </xsl:call-template>
    </box>
    <box class="PrioritySignal" name="chalk">
      <xsl:call-template name="extract-setpoints">
        <xsl:with-param name="climateSetpointName" select="'Cli_ShadingAgentReduction'"/>
      </xsl:call-template>
    </box>
    <box class="PrioritySignal" name="growthLightThresholdLow">
      <xsl:call-template name="extract-setpoints">
        <xsl:with-param name="climateSetpointName" select="'Cli_AssLightOn'"/>
      </xsl:call-template>
    </box>
    <box class="PrioritySignal" name="growthLightThresholdHigh">
      <xsl:call-template name="extract-setpoints">
        <xsl:with-param name="climateSetpointName" select="'Cli_AssLightOff'"/>
      </xsl:call-template>
    </box>
    <box class="PrioritySignal" name="growthLightActive">
      <xsl:call-template name="extract-setpoints">
        <xsl:with-param name="climateSetpointName" select="'Cli_AssLightActive'"/>
      </xsl:call-template>
    </box>
    <box class="PrioritySignal" name="rhMaxBand">
      <xsl:call-template name="extract-setpoints">
        <xsl:with-param name="climateSetpointName" select="'rhMaxBand'"/>
      </xsl:call-template>
    </box>
    <box class="PrioritySignal" name="dawnThreshold">
      <xsl:call-template name="extract-setpoints">
        <xsl:with-param name="climateSetpointName" select="'Cli_LightChangeFromNight'"/>
      </xsl:call-template>
    </box>
    <box class="PrioritySignal" name="duskThreshold">
      <xsl:call-template name="extract-setpoints">
        <xsl:with-param name="climateSetpointName" select="'Cli_LightChangeFromDay'"/>
      </xsl:call-template>
    </box>
    <box class="PrioritySignal" name="crackVentilation">
      <xsl:call-template name="extract-setpoints">
        <xsl:with-param name="climateSetpointName" select="'crackVentilation'"/>
      </xsl:call-template>
    </box>
    <box class="PrioritySignal" name="crackVentilationTemperatureMin">
      <xsl:call-template name="extract-setpoints">
        <xsl:with-param name="climateSetpointName" select="'crackVentilationTemperatureMin'"/>
      </xsl:call-template>
    </box>
    <box class="PrioritySignal" name="crackVentilationTemperatureMinBand">
      <xsl:call-template name="extract-setpoints">
        <xsl:with-param name="climateSetpointName" select="'crackVentilationTemperatureMinBand'"/>
      </xsl:call-template>
    </box>
    <box class="PrioritySignal" name="screenEnergyThreshold">
      <xsl:call-template name="extract-setpoints">
        <xsl:with-param name="climateSetpointName" select="'screenEnergyThreshold'"/>
      </xsl:call-template>
    </box>
    <box class="PrioritySignal" name="screenEnergyThresholdBand">
      <xsl:call-template name="extract-setpoints">
        <xsl:with-param name="climateSetpointName" select="'screenEnergyThresholdBand'"/>
      </xsl:call-template>
    </box>
    <box class="PrioritySignal" name="screenShadeThreshold">
      <xsl:call-template name="extract-setpoints">
        <xsl:with-param name="climateSetpointName" select="'screenShadeThreshold'"/>
      </xsl:call-template>
    </box>
    <box class="PrioritySignal" name="screenShadeThresholdBand">
      <xsl:call-template name="extract-setpoints">
        <xsl:with-param name="climateSetpointName" select="'screenShadeThresholdBand'"/>
      </xsl:call-template>
    </box>
    <box class="PrioritySignal" name="screenBlackoutFromTimeFloat">
      <xsl:call-template name="extract-setpoints">
        <xsl:with-param name="climateSetpointName" select="'screenBlackoutFromTime'"/>
      </xsl:call-template>
    </box>
    <box class="PrioritySignal" name="screenBlackoutToTimeFloat">
      <xsl:call-template name="extract-setpoints">
        <xsl:with-param name="climateSetpointName" select="'screenBlackoutToTime'"/>
      </xsl:call-template>
    </box>
    <box class="PrioritySignal" name="screenMaxAtHighRh">
      <xsl:call-template name="extract-setpoints">
        <xsl:with-param name="climateSetpointName" select="'screenMaxAtHighRh'"/>
      </xsl:call-template>
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
    <xsl:call-template name="setpoint-reference">
      <xsl:with-param name="setpointName" select="'ventilationThresholdBand'"/>
    </xsl:call-template>
    <xsl:call-template name="setpoint-reference">
      <xsl:with-param name="setpointName" select="'crackVentilation'"/>
    </xsl:call-template>
    <xsl:call-template name="setpoint-reference">
      <xsl:with-param name="setpointName" select="'crackVentilationTemperatureMin'"/>
    </xsl:call-template>
    <xsl:call-template name="setpoint-reference">
      <xsl:with-param name="setpointName" select="'crackVentilationTemperatureMinBand'"/>
    </xsl:call-template>
    <xsl:call-template name="setpoint-reference">
      <xsl:with-param name="setpointName" select="'screenMaxAtHighRh'"/>
    </xsl:call-template>
    <xsl:call-template name="setpoint-reference">
      <xsl:with-param name="setpointName" select="'screenEnergyThreshold'"/>
    </xsl:call-template>
    <xsl:call-template name="setpoint-reference">
      <xsl:with-param name="setpointName" select="'screenEnergyThresholdBand'"/>
    </xsl:call-template>
    <xsl:call-template name="setpoint-reference">
      <xsl:with-param name="setpointName" select="'screenShadeThreshold'"/>
    </xsl:call-template>
    <xsl:call-template name="setpoint-reference">
      <xsl:with-param name="setpointName" select="'screenShadeThresholdBand'"/>
    </xsl:call-template>
    <xsl:call-template name="setpoint-reference">
      <xsl:with-param name="setpointName" select="'screenBlackoutFromTimeFloat'"/>
    </xsl:call-template>
    <xsl:call-template name="setpoint-reference">
      <xsl:with-param name="setpointName" select="'screenBlackoutToTimeFloat'"/>
    </xsl:call-template>
    <xsl:call-template name="setpoint-reference">
      <xsl:with-param name="setpointName" select="'chalk'"/>
    </xsl:call-template>
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
      <port name="maxSignal" externalName="None" source="Fixed">
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
      <newPort name="value" externalName="None" source="Fixed" value="1"/>
      <newPort name="fractionPlantArea" externalName="None" source="Fixed" value="1"/>
    </box>
  </box>
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
      <newPort name="leafNetPhotosynthesis" ref="crop/growth[netGrowthRate]"/>
      <newPort name="leafLightUseEfficiencyTop" ref="crop/layers/top/photosynthesis/lightResponse[LUE]"/>
      <newPort name="leafLightUseEfficiencyMiddle" ref="crop/layers/middle/photosynthesis/lightResponse[LUE]"/>
      <newPort name="leafLightUseEfficiencyBottom" ref="crop/layers/bottom/photosynthesis/lightResponse[LUE]"/>
      <newPort name="leafTemperatureTop"    ref="crop/layers/top/temperature[value]"/>
      <newPort name="leafTemperatureMiddle" ref="crop/layers/middle/temperature[value]"/>
      <newPort name="leafTemperatureBottom" ref="crop/layers/bottom/temperature[value]"/>
      <newPort name="setpointVentilation" ref="setpoints/temperature/ventilation[value]"/>
      <newPort name="setpointHeating" ref="setpoints/temperature/heating[value]"/>
      <newPort name="setpointCo2Minimum" ref="setpoints/co2/minimum[signal]"/>
      <newPort name="setpointCo2Maximum" ref="setpoints/co2/maximum[signal]"/>
      <newPort name="ventsOpening" ref="actuators/vents[value]"/>
      <newPort name="growthLightOn" ref="controllers/growthLight[value]"/>
      <newPort name="vapourFluxTranspiration" ref="indoors/given/vapourFlux/transpiration[vapourFlux]"/>
      <newPort name="vapourFluxCondensationCover" ref="indoors/given/vapourFlux/condensationCover[vapourFlux]"/>
      <newPort name="vapourFluxCondensationsScreens" ref="indoors/given/vapourFlux/condensationScreens[vapourFlux]"/>
      <newPort name="vapourFluxLeakage" ref="indoors/given/vapourFlux/airFluxOutdoors[vapourFlux]"/>
      <newPort name="vapourFluxVents" ref="cooling/vapourFlux[vapourFlux]"/>
    </box>
    <box class="PageR">
      <port name="xAxis" value="calendar[dateTime]"/>
      <box class="PlotR">
        <port name="ncol" value="4"/>
        <port name="ports" value="output/p[*]"/>
      </box>
    </box>
    <xsl:variable name="TimeStep" select="DVV_SETUP/TimeStep"/>
    <box class="OutputText">
      <port name="skipFormats" value="TRUE"/>
      <port name="skipInitialRows">
        <xsl:attribute name="value">
          <xsl:value-of select="1440 div number($TimeStep)"/>
        </xsl:attribute>
      </port>
      <port name="averageN">
        <xsl:attribute name="value">
          <xsl:value-of select="60 div number($TimeStep)"/>
        </xsl:attribute>
      </port>
    </box>
  </box>
  
  </box> </xsl:template>

</xsl:stylesheet>
