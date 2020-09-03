<xsl:stylesheet version="2.0" 
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:ecolmod="http://www.ecolmod.org">
<xsl:output indent="yes" encoding="ISO-8859-1"/>

<!-- Helpers -->
<xsl:variable name="colon" select="codepoints-to-string(58)"/>
<xsl:variable name="test-weather-file" select="'input/sel_dk.txt'"/> 

<!-- Missing parameters -->
<xsl:variable name="HPS-minPeriodOn" select="30"/>
<xsl:variable name="LED-minPeriodOn" select="0"/>
<xsl:variable name="UwindExponent" select="0.8"/>

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
  <xsl:param name="pNode"/> <!--as="node()"/>-->

  <xsl:for-each select="$pNode/ancestor::*">    
    <xsl:value-of select="concat('/', name())" />    
  </xsl:for-each>    
    <xsl:value-of select="concat('/', name($pNode[1]))" />     
</xsl:function>

<xsl:template name="extract-cover">
  <xsl:param name="cover"/>

  <xsl:variable name="swReflectivityName" select="'PaneReflection'"/>
  <xsl:variable name="swReflectivitySrc" select="$cover/Constants/Parameters[ParameterName=$swReflectivityName]/Value" as="node()"/>
  <xsl:variable name="swTransmissivityName" select="'PaneTransmission'"/>
  <xsl:variable name="swTransmissivitySrc" select="$cover/Constants/Parameters[ParameterName=$swTransmissivityName]/Value" as="node()"/>
  
  <xsl:variable name="lwReflectivityName" select="'PaneLwReflection'"/>
  <xsl:variable name="lwReflectivitySrc" select="$cover/Constants/Parameters[ParameterName=$lwReflectivityName]/Value" as="node()"/>
  <xsl:variable name="lwTransmissivityName" select="'PaneLwTransmission'"/>
  <xsl:variable name="lwTransmissivitySrc" select="$cover/Constants/Parameters[ParameterName=$lwTransmissivityName]/Value" as="node()"/>
  
  <xsl:variable name="UwindMinimumName" select="'PaneUValue'"/>
  <xsl:variable name="UwindMinimumSrc" select="$cover/Constants/Parameters[ParameterName=$UwindMinimumName]/Value" as="node()"/>

  <xsl:variable name="UwindSlopeName" select="'UwindSlope'"/>
  <xsl:variable name="UwindSlopeSrc" select="DVV_SETUP/Greenhouse/Constants/Parameters[ParameterName=$UwindSlopeName]/Value" as="node()"/>
  <xsl:variable name="UwindSlopeValue" select="number(replace($UwindSlopeSrc, ',', '.'))"/>

  <box class="vg::Cover" name="cover">
    <port name="swReflectivityTop">
      <xsl:attribute name="externalName">
        <xsl:value-of select="$swReflectivityName"/>
      </xsl:attribute>
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($swReflectivitySrc)"/>
      </xsl:attribute>
      <xsl:call-template name="float-value">
        <xsl:with-param name="value" select="$swReflectivitySrc"/>
      </xsl:call-template>
    </port>
    <port name="swReflectivityBottom">
      <xsl:attribute name="externalName">
        <xsl:value-of select="$swReflectivityName"/>
      </xsl:attribute>
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($swReflectivitySrc)"/>
      </xsl:attribute>
      <xsl:call-template name="float-value">
        <xsl:with-param name="value" select="$swReflectivitySrc"/>
      </xsl:call-template>
    </port>
    <port name="swTransmissivityTop">
      <xsl:attribute name="externalName">
        <xsl:value-of select="$swTransmissivityName"/>
      </xsl:attribute>
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($swTransmissivitySrc)"/>
      </xsl:attribute>
      <xsl:call-template name="float-value">
        <xsl:with-param name="value" select="$swTransmissivitySrc"/>
      </xsl:call-template>
    </port>
    <port name="swTransmissivityBottom">
      <xsl:attribute name="externalName">
        <xsl:value-of select="$swTransmissivityName"/>
      </xsl:attribute>
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($swTransmissivitySrc)"/>
      </xsl:attribute>
      <xsl:call-template name="float-value">
        <xsl:with-param name="value" select="$swTransmissivitySrc"/>
      </xsl:call-template>
    </port>

    <port name="lwReflectivityTop">
      <xsl:attribute name="externalName">
        <xsl:value-of select="$lwReflectivityName"/>
      </xsl:attribute>
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($lwReflectivitySrc)"/>
      </xsl:attribute>
      <xsl:call-template name="float-value">
        <xsl:with-param name="value" select="$lwReflectivitySrc"/>
      </xsl:call-template>
    </port>
    <port name="lwReflectivityBottom">
      <xsl:attribute name="externalName">
        <xsl:value-of select="$lwReflectivityName"/>
      </xsl:attribute>
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($lwReflectivitySrc)"/>
      </xsl:attribute>
      <xsl:call-template name="float-value">
        <xsl:with-param name="value" select="$lwReflectivitySrc"/>
      </xsl:call-template>
    </port>
    <port name="lwTransmissivityTop">
      <xsl:attribute name="externalName">
        <xsl:value-of select="$lwTransmissivityName"/>
      </xsl:attribute>
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($lwTransmissivitySrc)"/>
      </xsl:attribute>
      <xsl:call-template name="float-value">
        <xsl:with-param name="value" select="$lwTransmissivitySrc"/>
      </xsl:call-template>
    </port>
    <port name="lwTransmissivityBottom">
      <xsl:attribute name="externalName">
        <xsl:value-of select="$lwTransmissivityName"/>
      </xsl:attribute>
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($lwTransmissivitySrc)"/>
      </xsl:attribute>
      <xsl:call-template name="float-value">
        <xsl:with-param name="value" select="$lwTransmissivitySrc"/>
      </xsl:call-template>
    </port>
    
    <port name="UwindMinimum">
      <xsl:attribute name="externalName">
        <xsl:value-of select="$UwindMinimumName"/>
      </xsl:attribute>
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($UwindMinimumSrc)"/>
      </xsl:attribute>
      <xsl:call-template name="float-value">
        <xsl:with-param name="value" select="$UwindMinimumSrc"/>
      </xsl:call-template>
    </port>
    <port name="UwindSlope">
      <xsl:attribute name="externalName">
        <xsl:value-of select="$UwindSlopeName"/>
      </xsl:attribute>
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($UwindSlopeSrc)"/>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:value-of select="$UwindSlopeValue"/>
      </xsl:attribute>
    </port>
    <port name="UwindExponent" externalName="UwindExponent" source="Fixed">
      <xsl:attribute name="value">
        <xsl:value-of select="$UwindExponent"/>
      </xsl:attribute>
    </port>
  </box>
</xsl:template>

<xsl:template name="extract-screen">
  <xsl:param name="position"/>
  <xsl:if test="Constants/Parameters[ParameterName='ScreenProduct']/Value!='No screen'">
    <xsl:variable name="layer" select="Layer"/>
    <xsl:variable name="reflectivityTopName" select="'ReflectionOutwards'"/>
    <xsl:variable name="reflectivityTopSrc" select="Constants/Parameters[ParameterName=$reflectivityTopName]/Value" as="node()"/>
    <xsl:variable name="reflectivityBottomName" select="'ReflectionInwards'"/>
    <xsl:variable name="reflectivityBottomSrc" select="Constants/Parameters[ParameterName=$reflectivityBottomName]/Value" as="node()"/>
    <xsl:variable name="transmissivityName" select="'Transmission'"/>
    <xsl:variable name="transmissivitySrc" select="Constants/Parameters[ParameterName=$transmissivityName]/Value" as="node()"/>
    <xsl:variable name="UName" select="'U'"/>
    <xsl:variable name="USrc" select="Constants/Parameters[ParameterName=$UName]/Value" as="node()"/>
    <xsl:variable name="transmissivityAirName" select="'Porosity'"/>
    <xsl:variable name="transmissivityAirSrc" select="Constants/Parameters[ParameterName=$transmissivityAirName]/Value" as="node()"/>
          
    <box name="MISSING">
    
      <xsl:choose>  
        <xsl:when test="$position=1 or $position=2">
          <xsl:attribute name="class">
            <xsl:value-of select="'vg::ScreenRoof'"/>
          </xsl:attribute>
        </xsl:when>
        <xsl:otherwise>
          <xsl:attribute name="class">
            <xsl:value-of select="'vg::ScreenWall'"/>
          </xsl:attribute>
        </xsl:otherwise>
      </xsl:choose>  
      
      <xsl:attribute name="name">
        <xsl:value-of select="concat('layer', $layer)"/>
      </xsl:attribute>
      
      <port name="swTransmissivityTop">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$transmissivityName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($transmissivitySrc)"/>
        </xsl:attribute>
        <xsl:call-template name="float-value">
          <xsl:with-param name="value" select="$transmissivitySrc"/>
        </xsl:call-template>
      </port>
      <port name="swTransmissivityBottom">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$transmissivityName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($transmissivitySrc)"/>
        </xsl:attribute>
        <xsl:call-template name="float-value">
          <xsl:with-param name="value" select="$transmissivitySrc"/>
        </xsl:call-template>
      </port>
      <port name="swReflectivityTop">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$reflectivityTopName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($reflectivityTopSrc)"/>
        </xsl:attribute>
        <xsl:call-template name="float-value">
          <xsl:with-param name="value" select="$reflectivityTopSrc"/>
        </xsl:call-template>
      </port>
      <port name="swReflectivityBottom">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$reflectivityBottomName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($reflectivityBottomSrc)"/>
        </xsl:attribute>
        <xsl:call-template name="float-value">
          <xsl:with-param name="value" select="$reflectivityBottomSrc"/>
        </xsl:call-template>
      </port>
      <port name="lwTransmissivityTop">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$transmissivityName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($transmissivitySrc)"/>
        </xsl:attribute>
        <xsl:call-template name="float-value">
          <xsl:with-param name="value" select="$transmissivitySrc"/>
        </xsl:call-template>
      </port>
      <port name="lwTransmissivityBottom">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$transmissivityName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($transmissivitySrc)"/>
        </xsl:attribute>
        <xsl:call-template name="float-value">
          <xsl:with-param name="value" select="$transmissivitySrc"/>
        </xsl:call-template>
      </port>

      <port name="lwReflectivityTop">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$reflectivityTopName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($reflectivityTopSrc)"/>
        </xsl:attribute>
        <xsl:call-template name="float-value">
          <xsl:with-param name="value" select="$reflectivityTopSrc"/>
        </xsl:call-template>
      </port>
      <port name="lwReflectivityBottom">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$reflectivityBottomName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($reflectivityBottomSrc)"/>
        </xsl:attribute>
        <xsl:call-template name="float-value">
          <xsl:with-param name="value" select="$reflectivityBottomSrc"/>
        </xsl:call-template>
      </port>

      <port name="Utop">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$UName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($USrc)"/>
        </xsl:attribute>
        <xsl:call-template name="float-value">
          <xsl:with-param name="value" select="$USrc"/>
        </xsl:call-template>
      </port>
      <port name="Ubottom">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$UName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($USrc)"/>
        </xsl:attribute>
        <xsl:call-template name="float-value">
          <xsl:with-param name="value" select="$USrc"/>
        </xsl:call-template>
      </port>
      
      <port name="transmissivityAir">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$transmissivityAirName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($transmissivityAirSrc)"/>
        </xsl:attribute>
        <xsl:call-template name="float-value">
          <xsl:with-param name="value" select="$transmissivityAirSrc"/>
        </xsl:call-template>
      </port>
      
      <port name="state">
        <xsl:attribute name="ref">
          <xsl:value-of select="concat('actuators/screens/layer', $layer, '[value]')"/>
        </xsl:attribute>
        <!-- <xsl:attribute name="ref"> -->
          <!-- <xsl:value-of select="concat('actuators/screens/', $name, '[value]')"/> -->
        <!-- </xsl:attribute> -->
      </port>
      <box class="vg::ScreensAirTransmissivity" name="airTransmissivity"/>
    </box>
  </xsl:if>
</xsl:template>

<xsl:template name="extract-screens">
  <xsl:param name="position"/>
  <xsl:for-each select=".[Layer='1']">
    <xsl:call-template name="extract-screen">
      <xsl:with-param name="position" select="$position"/>
    </xsl:call-template>
  </xsl:for-each>
  <xsl:for-each select=".[Layer='2']">
    <xsl:call-template name="extract-screen">
      <xsl:with-param name="position" select="$position"/>
    </xsl:call-template>
  </xsl:for-each>
  <xsl:for-each select=".[Layer='3']">
    <xsl:call-template name="extract-screen">
      <xsl:with-param name="position" select="$position"/>
    </xsl:call-template>
  </xsl:for-each>
</xsl:template>

<xsl:template name="extract-shelter-face">
  <xsl:param name="position"/>
  <box class="vg::ShelterFaceArea" name="area"/>
  <xsl:call-template name="extract-cover">
    <xsl:with-param name="cover" select="DVV_SETUP/Greenhouse/Panes/Pane[Position=$position]"/>
  </xsl:call-template>
  <box class="vg::Screens" name="screens">
    <xsl:for-each select="DVV_SETUP/Greenhouse/Screens/Screen[Position=$position]">
      <xsl:call-template name="extract-screens">
        <xsl:with-param name="position" select="$position"/>
      </xsl:call-template>
    </xsl:for-each>
  </box>
</xsl:template>

<xsl:template name="extract-vent">
  <xsl:param name="position"/>
  <!-- <xsl:variable name="ventName" select="concat('vent', $position)"/> -->
  <xsl:variable name="ventName" select="'vent'"/>
  <xsl:variable name="lengthName" select="'Length'"/>
  <xsl:variable name="lengthSrc" select="DVV_SETUP/Greenhouse/Vents/Vent[Position=$position]/Constants/Parameters[ParameterName=$lengthName]/Value"/>
  <xsl:variable name="lengthValue" select="number(replace($lengthSrc, ',', '.'))"/>
  <xsl:variable name="widthName" select="'Width'"/>
  <xsl:variable name="widthSrc" select="DVV_SETUP/Greenhouse/Vents/Vent[Position=$position]/Constants/Parameters[ParameterName=$widthName]/Value"/>
  <xsl:variable name="widthValue" select="number(replace($widthSrc, ',', '.'))"/>
  <xsl:variable name="numberName" select="'Number'"/>
  <xsl:variable name="numberSrc" select="DVV_SETUP/Greenhouse/Vents/Vent[Position=$position]/Constants/Parameters[ParameterName=$numberName]/Value"/>
  <xsl:variable name="numberValue" select="number(replace($numberSrc, ',', '.'))"/>
  <xsl:variable name="transmissivityName" select="'VentTransmissivity'"/>
  <xsl:variable name="transmissivitySrc" select="DVV_SETUP/Greenhouse/Vents/Vent[Position=$position]/Constants/Parameters[ParameterName=$transmissivityName]/Value"/>
  <xsl:variable name="transmissivityValue" select="number(replace($transmissivitySrc, ',', '.'))"/>

  <box class="vg::Vent" name="vent">
    <xsl:attribute name="name">
      <xsl:value-of select="$ventName"/>
    </xsl:attribute>
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
      <newPort name="flagIsUp" value="TRUE"/>
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
    <box class="DateTimeSignal">
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

<!-- <xsl:template name="setpoint-reference"> -->
  <!-- <xsl:param name="setpointName"/> -->
  <!-- <port> -->
    <!-- <xsl:attribute name="name"> -->
      <!-- <xsl:value-of select="$setpointName"/> -->
    <!-- </xsl:attribute> -->
    <!-- <xsl:attribute name="ref"> -->
      <!-- <xsl:value-of select="concat('allSetpoints/', $setpointName, '[value]')"/> -->
    <!-- </xsl:attribute> -->
  <!-- </port> -->
<!-- </xsl:template> -->

<xsl:template name="extract-growth-light">
  <xsl:variable name="position" select="Position"/>
  <xsl:variable name="growthLightName" select="concat('growthLight', $position)"/>
  <xsl:variable name="typeName" select="'LampType'"/>
  <xsl:variable name="typeSrc" select="Constants/Parameters[ParameterName=$typeName]/Value"/>
  <xsl:variable name="intensityName" select="'LightCapacityPerSqm'"/>
  <xsl:variable name="intensitySrc" select="Constants/Parameters[ParameterName=$intensityName]/Value"/>
  <xsl:variable name="intensityValue" select="number(replace($intensitySrc, ',', '.'))"/>
  <xsl:variable name="parPhotonCoefName" select="'MicromolParPerWatt'"/>
  <xsl:variable name="parPhotonCoefSrc" select="Constants/Parameters[ParameterName=$parPhotonCoefName]/Value"/>
  <xsl:variable name="parPhotonCoefValue" select="number(replace($parPhotonCoefSrc, ',', '.'))"/>
  <xsl:variable name="ageCorrectedEfficiencyName" select="'ageCorrectedEfficiency'"/>
  <xsl:variable name="ageCorrectedEfficiencySrc" select="Constants/Parameters[ParameterName=$ageCorrectedEfficiencyName]/Value"/>
  <xsl:variable name="ageCorrectedEfficiencyValue" select="number(replace($ageCorrectedEfficiencySrc, ',', '.'))"/>

  <xsl:if test="$intensityValue > 0">
    <box class="vg::GrowthLight">
      <xsl:attribute name="name">
        <xsl:value-of select="$growthLightName"/>
      </xsl:attribute>
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
      
      <port name="on">
        <xsl:attribute name="ref">
          <xsl:value-of select="concat('controllers/growthLights/', $growthLightName, '[flagIsUp]')"/>
        </xsl:attribute>
      </port> 
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
      <port name="ageCorrectedEfficiency">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$ageCorrectedEfficiencyName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($ageCorrectedEfficiencySrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$ageCorrectedEfficiencyValue"/>
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

<xsl:template name="actuator-layer">
  <xsl:param name="layer"/>
  <xsl:variable name="model-name" select="concat('screenActive', $layer)"/>
  <xsl:variable name="model"
                 select="DVV_SETUP/Greenhouse/Climate/Setpoint/Constants/Parameters[ParameterName=$model-name]/Value"/>
  <box class="Accumulator"> 
    <xsl:attribute name="name">
      <xsl:value-of select="concat('layer', $layer)"/> 
    </xsl:attribute>
    <port name="change" ref="./controller[controlVariable]"/>
    <port name="minValue" value="0"/>
    <port name="maxValue" ref="setpoints[maxScreen]"/>
    <box class="ScreenCombination" name="desiredValue">
      <port name="formula">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$model-name"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$model"/>
        </xsl:attribute>
      </port>
    </box>
    <box class="PidController" name="controller">
      <!-- <port name="desiredValue" externalName="screenActive1"> -->
        <!-- <xsl:attribute name="ref"> -->
          <!-- <xsl:value-of select="concat('controllers/screens/', $model, '[value]')"/> -->
        <!-- </xsl:attribute> -->
      <!-- </port> -->
      <port name="desiredValue" ref="../desiredValue[value]"/>
      <port name="sensedValue" ref="..[value]"/>
      <port name="Kprop" value="0.05"/>
    </box>
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
  <xsl:variable name="floorReflectivityName" select="'floor-reflectance'"/>
  <xsl:variable name="floorReflectivitySrc" select="DVV_SETUP/Greenhouse/Constants/Parameters[ParameterName=$floorReflectivityName]/Value" as="node()"/>
  <xsl:variable name="floorReflectivityValue" select="number(replace($floorReflectivitySrc, ',', '.'))"/>
  <xsl:variable name="floorUtopName" select="'floor-Uindoors'"/>
  <xsl:variable name="floorUtopSrc" select="DVV_SETUP/Greenhouse/Constants/Parameters[ParameterName=$floorUtopName]/Value" as="node()"/>
  <xsl:variable name="floorUtopValue" select="number(replace($floorUtopSrc, ',', '.'))"/>
  <xsl:variable name="floorUbottomName" select="'floor-Usoil'"/>
  <xsl:variable name="floorUbottomSrc" select="DVV_SETUP/Greenhouse/Constants/Parameters[ParameterName=$floorUbottomName]/Value" as="node()"/>
  <xsl:variable name="floorUbottomValue" select="number(replace($floorUbottomSrc, ',', '.'))"/>
  <xsl:variable name="floorHeatCapacityName" select="'floor-heatCapacity'"/>
  <xsl:variable name="floorHeatCapacitySrc" select="DVV_SETUP/Greenhouse/Constants/Parameters[ParameterName=$floorHeatCapacityName]/Value" as="node()"/>
  <xsl:variable name="floorHeatCapacityValue" select="number(replace($floorHeatCapacitySrc, ',', '.'))"/>

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
        <xsl:call-template name="extract-vent">
          <xsl:with-param name="position" select="1"/>
        </xsl:call-template>
      </box>
      <box class="vg::ShelterFace" name="roof2">
        <xsl:call-template name="extract-shelter-face">
          <xsl:with-param name="position" select="2"/>
        </xsl:call-template>
        <xsl:call-template name="extract-vent">
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
    <box name="floor">
      <newPort name="reflectivity">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$floorReflectivityName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($floorReflectivitySrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$floorReflectivityValue"/>
        </xsl:attribute>
      </newPort>
      <newPort name="Utop">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$floorUtopName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($floorUtopSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$floorUtopValue"/>
        </xsl:attribute>
      </newPort>
      <newPort name="Ubottom">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$floorUbottomName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($floorUbottomSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$floorUbottomValue"/>
        </xsl:attribute>
      </newPort>
      <newPort name="heatCapacity">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$floorHeatCapacityName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($floorHeatCapacitySrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$floorHeatCapacityValue"/>
        </xsl:attribute>
      </newPort>
    </box>
  
  </box>

  <xsl:comment> *** Setpoints *** </xsl:comment>
  <box class="vg::Setpoints" name="setpoints">
    <box name="elementary">
      <box class="PrioritySignal" name="heatingTemperatureNormalRh">
        <port name="initialSignal" value="20"/>  
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'HeatingTemp'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal"  name="heatingTemperatureHighRhMargin">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'MaxHeatAddHighRH'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal"  name="heatingTemperatureLowRhMargin">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'MaxHeatAddLowRH'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal"  name="rhMax">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'MaxRelHmd'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal" name="rhMaxBand">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'rhMaxBand'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal"  name="rhMin">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'MinRelHmd'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal" name="rhMinBand">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'rhMinBand'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal"  name="ventilationTemperatureMargin">
        <port name="initialSignal" value="5"/>  
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'VentTemp'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal" name="crackVentilationNormalTemperature">
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
      <box class="PrioritySignal" name="screenCrackAtHighRh">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'crackScreenHighHumidity'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal" name="screenCrackAtHighTemperature">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'crackScreenHighTemperature'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal" name="screenCrackAtHighTemperatureBand">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'crackScreenHighTemperatureBand'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal" name="co2Capacity">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'CO2Capacity'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal" name="co2Setpoint">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'CO2Setpoint'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal" name="co2VentilationThreshold">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'CO2VentilationThreshold'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal" name="co2VentilationBand">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'CO2VentilationBand'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal" name="chalk">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'ShadingAgentReduction'"/>
        </xsl:call-template>
      </box>
      <box name="growthLights">
        <box name= "growthLight1">
          <box class="PrioritySignal" name="thresholdLow">
            <xsl:call-template name="extract-setpoints">
              <xsl:with-param name="climateSetpointName" select="'AssLightOn1'"/>
            </xsl:call-template>
          </box>
          <box class="PrioritySignal" name="thresholdHigh">
            <xsl:call-template name="extract-setpoints">
              <xsl:with-param name="climateSetpointName" select="'AssLightOff1'"/>
            </xsl:call-template>
          </box>
          <box class="PrioritySignal" name="setting">
            <xsl:call-template name="extract-setpoints">
              <xsl:with-param name="climateSetpointName" select="'AssLightActive1'"/>
            </xsl:call-template>
          </box>
        </box>
        <box name= "growthLight2">
          <box class="PrioritySignal" name="thresholdLow">
            <xsl:call-template name="extract-setpoints">
              <xsl:with-param name="climateSetpointName" select="'AssLightOn2'"/>
            </xsl:call-template>
          </box>
          <box class="PrioritySignal" name="thresholdHigh">
            <xsl:call-template name="extract-setpoints">
              <xsl:with-param name="climateSetpointName" select="'AssLightOff2'"/>
            </xsl:call-template>
          </box>
          <box class="PrioritySignal" name="setting">
            <xsl:call-template name="extract-setpoints">
              <xsl:with-param name="climateSetpointName" select="'AssLightActive2'"/>
            </xsl:call-template>
          </box>
        </box>
        <box name= "growthLight3">
          <box class="PrioritySignal" name="thresholdLow">
            <xsl:call-template name="extract-setpoints">
              <xsl:with-param name="climateSetpointName" select="'AssLightOn3'"/>
            </xsl:call-template>
          </box>
          <box class="PrioritySignal" name="thresholdHigh">
            <xsl:call-template name="extract-setpoints">
              <xsl:with-param name="climateSetpointName" select="'AssLightOff3'"/>
            </xsl:call-template>
          </box>
          <box class="PrioritySignal" name="setting">
            <xsl:call-template name="extract-setpoints">
              <xsl:with-param name="climateSetpointName" select="'AssLightActive3'"/>
            </xsl:call-template>
          </box>
        </box>
      </box>
      <box class="PrioritySignal" name="screenEnergyThreshold1">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'screenEnergyThreshold1'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal" name="screenEnergyThreshold2">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'screenEnergyThreshold2'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal" name="screenEnergyThresholdBand">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'screenEnergyThresholdBand'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal" name="screenShadeThreshold1">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'screenShadeThreshold1'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal" name="screenShadeThreshold2">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'screenShadeThreshold2'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal" name="screenShadeThresholdBand">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'screenShadeThresholdBand'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal" name="screenFixed1">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'screenFixed1'"/>
        </xsl:call-template>
      </box>
    </box>
  </box>
  <xsl:comment> *** Controllers *** </xsl:comment>
  <box class="vg::Controllers" name="controllers">
    <box name="co2Injection">
      <newPort name="value" ref="./controller[controlVariable]"/>
      <box class="PidController" name="controller">
        <port name="sensedValue" ref="indoors/co2[value]"/>
        <port name="desiredValue" ref="setpoints[co2Setpoint]"/>
        <port name="Kprop" value="0.05"/>
        <port name="lookAhead" value="10"/>
      </box>
    </box>
  </box>

  <xsl:comment> *** Actuators *** </xsl:comment>
  <xsl:variable name="heatPipePropConvectionName" select="'HeatPipeConvection'"/>
  <xsl:variable name="heatPipePropConvectionSrc" select="DVV_SETUP/Greenhouse/Constants/Parameters[ParameterName=$heatPipePropConvectionName]/Value" as="node()"/>
  <xsl:variable name="heatPipePropConvectionValue" select="number(replace($heatPipePropConvectionSrc, ',', '.'))"/>

  <box class="vg::Actuators" name="actuators">
    <box class="ActuatorHeatPipes" name="heating">
    <port name="propConvection" externalName="propConvection" source="Fixed">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$heatPipePropConvectionName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($heatPipePropConvectionSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$heatPipePropConvectionValue"/>
        </xsl:attribute>
    </port>
      <box name="pipes">
        <xsl:for-each select="DVV_SETUP/Greenhouse/Heatpipes/Heatpipe">
          <xsl:call-template name="extract-heat-pipe"/>
        </xsl:for-each>
      </box>
    </box>

    <box class="ActuatorVentilation" name="ventilation"/>

    <box class="Accumulator" name="co2Injection">
      <port name="initial" value="0"/>
      <port name="change"   ref="controllers/co2Injection[value]"/>
      <port name="minValue"  value="0"/>
      <port name="maxValue"  ref="setpoints[co2Capacity]"/>
    </box>

    <box name="screens">
      <xsl:call-template name="actuator-layer">
        <xsl:with-param name="layer" select="1"/>
      </xsl:call-template>
      <xsl:call-template name="actuator-layer">
        <xsl:with-param name="layer" select="2"/>
      </xsl:call-template>
      <xsl:call-template name="actuator-layer">
        <xsl:with-param name="layer" select="3"/>
      </xsl:call-template>
    </box>
    <box class="GrowthLights" name="growthLights">
      <xsl:for-each select="DVV_SETUP/Greenhouse/Lamps/Lamp">
        <xsl:call-template name="extract-growth-light"/>
      </xsl:for-each>
    </box>
  </box>

  <xsl:comment> *** Climate variables *** </xsl:comment>
  <xsl:variable name="leakageName" select="'leakage'"/>
  <xsl:variable name="leakageSrc" select="DVV_SETUP/Greenhouse/Constants/Parameters[ParameterName=$leakageName]/Value" as="node()"/>
  <xsl:variable name="leakageValue" select="number(replace($leakageSrc, ',', '.'))"/>
  
  <box class="vg::EnergyBudgetOptimiser" name="energyBudgetOptimiser">
    <box class="vg::EnergyBudget" name="energyBudget">
    </box>
    <box name="indoors">
      <box class="vg::IndoorsVentilation" name="ventilation">
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
      <box class="vg::IndoorsTemperature" name="temperature">
      </box>
    </box>
  </box>
  
  <box class="WaterBudget" name="waterBudget"/>
  <box class="Indoors" name="indoors"/>
  
  <xsl:comment> *** Crop *** </xsl:comment>
  <xsl:variable name="laiName" select="'LAI'"/>
  <xsl:variable name="laiSrc" select="DVV_SETUP/Greenhouse/Crop/Parameters[ParameterName=$laiName]/Value" as="node()"/>
  <xsl:variable name="laiValue" select="number(replace($laiSrc, ',', '.'))"/>
  <xsl:variable name="coverageName" select="'propAreaCultured'"/>
  <xsl:variable name="coverageSrc" select="DVV_SETUP/Greenhouse/Crop/Parameters[ParameterName=$coverageName]/Value" as="node()"/>
  <xsl:variable name="coverageValue" select="number(replace($coverageSrc, ',', '.'))"/>
  <box class="vg::Crop" name="crop">
    <port name="lai">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$laiName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($laiSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$laiValue"/>
        </xsl:attribute>
    </port>
    <port name="coverage">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$coverageName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($coverageSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$coverageValue"/>
        </xsl:attribute>
    </port>
  </box>
  <xsl:comment> *** Budget *** </xsl:comment>
  <box class="vg::Budget" name="budget"/>
  <xsl:comment> *** Output *** </xsl:comment>
  <box class="OutputR" name="output">
    <box name="p">
       <newPort name="skyT" ref="outdoors[skyTemperature]"/>
      <newPort name="outdoorsT" ref="outdoors[temperature]"/>
      <newPort name="outdoorsRh" ref="outdoors[rh]"/>
      <newPort name="outdoorsLight" ref="outdoors[radiation]"/>
      <newPort name="outdoorsWindSpeed" ref="outdoors[windSpeed]"/>
      <newPort name="soilT" ref="outdoors[soilTemperature]"/>

      <newPort name="coverT" ref="energyBudget/cover[temperature]"/>
      <newPort name="screen1T" ref="energyBudget/screen1[temperature]"/>
      <newPort name="screen2T" ref="energyBudget/screen2[temperature]"/>
      <newPort name="screen3T" ref="energyBudget/screen3[temperature]"/>
       <newPort name="pipeT" ref="energyBudget/pipe[temperature]"/>
       <newPort name="leafT" ref="energyBudget/crop[temperature]"/>
       <newPort name="floorT" ref="energyBudget/floor[temperature]"/>

      <newPort name="indoorsT" ref="indoors/temperature[value]"/>
      <newPort name="indoorsRh" ref="indoors/humidity[rh]"/>
      <newPort name="indoorsSunPar" ref="energyBudget/sky[parFluxDown]"/>
       <newPort name="indoorsCo2" ref="indoors/co2[value]"/>

      <newPort name="Uinside" ref="energyBudget[Uinside]"/>
      <newPort name="Uoutside" ref="energyBudget[Uoutside]"/>
    
      <newPort name="spHeatingTemperature" ref="setpoints/heatingTemperature[value]"/>
      <newPort name="spVentilationTemperature" ref="setpoints/ventilationTemperature[value]"/>
      <newPort name="spRhMax" ref="setpoints[rhMax]"/>
      <newPort name="spRhMin" ref="setpoints[rhMin]"/>
      <newPort name="spCrackVentilation" ref="setpoints/crackVentilation[value]"/>
      <newPort name="spScreenEnergyThreshold1" ref="setpoints[screenEnergyThreshold1]"/>
      <newPort name="spScreenEnergyThreshold2" ref="setpoints[screenEnergyThreshold2]"/>
      <newPort name="spScreenShadeThreshold1" ref="setpoints[screenShadeThreshold1]"/>
      <newPort name="spScreenShadeThreshold2" ref="setpoints[screenShadeThreshold2]"/>
      <newPort name="spScreenFixed1" ref="setpoints[screenFixed1]"/>
      <newPort name="spScreenEnergyThresholdBand" ref="setpoints[screenEnergyThresholdBand]"/>
      <newPort name="spScreenShadeThresholdBand" ref="setpoints[screenShadeThresholdBand]"/>
      <newPort name="spScreenCrack" ref="setpoints/screenCrack[value]"/>
      <newPort name="spCo2" ref="setpoints[co2Setpoint]"/>

      <newPort name="actHeatingPower" ref="actuators/heating[energyFluxTotal]"/>
      <newPort name="actVentilation" ref="actuators/ventilation[value]"/>
      <newPort name="airFluxTotal" ref="indoors/ventilation[relative]"/>
      <newPort name="airFluxPower" ref="indoors/temperature[advectiveEnergyFlux]"/>

      <newPort name="actLight1" ref="actuators/growthLights/growthLight1[on]"/>
      <newPort name="actLight2" ref="actuators/growthLights/growthLight2[on]"/>
      <newPort name="actLight3" ref="actuators/growthLights/growthLight3[on]"/>
      <newPort name="actLightPower" ref="actuators/growthLights[powerUsage]"/>
      <newPort name="actLightPar" ref="actuators/growthLights[parFluxDown]"/>
      <newPort name="actScreen1" ref="actuators/screens/layer1[value]"/>
      <newPort name="actScreen2" ref="actuators/screens/layer2[value]"/>
      <newPort name="actScreen3" ref="actuators/screens/layer3[value]"/>
      <newPort name="actCo2" ref="actuators/co2Injection[value]"/>

      <newPort name="vapourFluxTranspiration" ref="waterBudget/transpiration[vapourFlux]"/>
      <newPort name="vapourFluxCondensationCrop" ref="waterBudget/condensationCrop[vapourFlux]"/>
      <newPort name="vapourFluxCondensationCover" ref="waterBudget/condensationCover[vapourFlux]"/>
      <newPort name="vapourFluxCondensationdScreens" ref="waterBudget/condensationScreens[vapourFlux]"/>
      <newPort name="vapourFluxVentilation" ref="waterBudget/ventilation[vapourFlux]"/>

      <newPort name="netPhotosynthesisRate" ref="crop/photosynthesis[Pn]"/>
      <newPort name="grossPhotosynthesisRate" ref="crop/photosynthesis[Pg]"/>
      <newPort name="darkRespirationRate" ref="crop/photosynthesis[Rd]"/>
      <newPort name="cropGrowthRate" ref="crop/growth[netGrowthRate]"/>
      <newPort name="lightUseEfficiency" ref="crop/photosynthesis[lue]"/>

      <newPort name="totalIrradiation"           ref="budget[skyIrradiation]"/>
      <newPort name="totalSkyAbsorbed"    ref="budget[skyRadiationAbsorbed]"/>
      <newPort name="totalSkySwAbsorbed"  ref="budget[skySwRadiationAbsorbed]"/>
      <newPort name="totalSkyLwAbsorbed"  ref="budget[skyLwRadiationAbsorbed]"/>
      <newPort name="totalVentilationEnergy"       ref="budget[ventilationEnergy]"/>
      <newPort name="totalConvectionEnergy"        ref="budget[convectionEnergy]"/>
      <newPort name="totalSoilEnergy"              ref="budget[soilEnergy]"/>
      <newPort name="totalHeatingEnergy"           ref="budget[heatingEnergy]"/>
      <newPort name="totalGrowthLightEnergy"     ref="budget[growthLightsEnergy]"/>
      <newPort name="totalParAbsorbed"     ref="budget[parAbsorbedTotal]"/>
      <newPort name="totalCo2"                ref="budget[co2Total]"/>
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
      <port name="useLocalDecimalChar" value="TRUE"/>
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
