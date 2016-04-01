<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output indent="yes" encoding="ISO-8859-1"/>

<!-- RESOURCES -->

<xsl:variable name="crops" select="document('crops.xml')/Crops"/>

<!-- CONFIGURATION -->

<!-- 0: File output for DVV; no progress bar
     1: Screen plot output for testing; show progress bar 
	 2: File output for testing; show progress bar -->
<xsl:variable name="test-mode" select="2"/>  

<!-- Simulation period used when test-mode > 0 -->
<xsl:variable name="BeginDate" select="'2001-01-01'"/>
<xsl:variable name="EndDate" select="'2001-12-31'"/>

<!-- Parameters missing in DVV Online must be set here -->
<xsl:variable name="EnergyScreenOption" select="1"/>  		<!-- 1: EnergyBalanc or 2: OutsideLight --> 
<xsl:variable name="spLightEnergyScreen" select="10"/>
<xsl:variable name="spShadowThresholdScreen" select="500"/>
<xsl:variable name="Use2ndScreenWithEnergy" select="1"/>
<xsl:variable name="MinHeatingPbandRH" select="5"/>			<!-- See GHValues.Control.Pipes.PbandRH -->
<xsl:variable name="MinVentilationPbandRH" select="5"/>		<!-- See GHValues.Control.Pipes.PbandRH -->
<xsl:variable name="HeatingLightAdjustmentThreshold" select="300"/>
<xsl:variable name="HeatingLightAdjustmentThresholdBand" select="100"/>
<xsl:variable name="HeatingLightAdjustment" select="1"/>
<xsl:variable name="OutdoorsCo2" select="400"/>
<xsl:variable name="Co2MaxSupply" select="4.5"/>				<!-- g/m2/h -->
<xsl:variable name="Co2VentilationThreshold" select="6"/>	<!-- h-1 -->
<xsl:variable name="Co2VentilationThresholdBand" select="1"/> <!-- h-1 -->

<xsl:variable name="CoverEmissitivity" select="0.93"/>  		<!-- [0;1] -->
<xsl:variable name="CoverAbsorptivity" select="0.04"/>		<!-- [0;1] -->
<xsl:variable name="CoverHaze" select="0"/>  					<!-- [0;1] -->
<xsl:variable name="CoverHeatCapacity" select="8736"/>		<!-- Heat capacity [J/m2/K] -->
																<!-- density*specHeatCapacity*thickness/1000  = 2600 kg/m3 * 840 J/kg/K * 4 mm *m/mm  -->
<xsl:variable name="ScreenHaze" select="0.8"/>				<!-- [0;1] -->
<xsl:variable name="ScreenEmissivityInner" select="0.62"/>	<!-- [0;1] -->
<xsl:variable name="ScreenEmissivityOuter" select="0.06"/>	<!-- [0;1] -->
<xsl:variable name="ScreenU" select="2"/>	                  

<xsl:variable name="MaxCrackVentilation" select="4"/>			<!-- [h-1] (GCC, p. 148) -->
<xsl:variable name="Leakage" select="1"/>						<!-- [h-1] -->

<xsl:variable name="FloorUindoors" select="7.5"/>	
<xsl:variable name="FloorUsoil" select="4"/>	
<xsl:variable name="FloorHeatCapacity" select="42000."/>	

		
<!-- GLOBAL VARIABLES -->

<!-- no. of spans * span width * length -->
<xsl:variable name="greenhouse-area" 
	select=
	"JobDataSet/Greenhouse/constants/Parameters/Parameters[ParameterID='9']/Value *  	
	 JobDataSet/Greenhouse/constants/Parameters/Parameters[ParameterID='11']/Value *	
	 JobDataSet/Greenhouse/constants/Parameters/Parameters[ParameterID='223']/Value">
</xsl:variable>

<xsl:variable name="crop-name">
	<xsl:variable name="cropType" select="//Cultures/Culture/Id"/>
	<xsl:choose>
		<xsl:when test="$cropType=1">Roses</xsl:when>
		<xsl:when test="$cropType=2">Chrysanthemum</xsl:when>
		<xsl:when test="$cropType=3">Pansy</xsl:when>
		<xsl:when test="$cropType=4">Tomato</xsl:when>
		<xsl:when test="$cropType=22">TomatoYearRound</xsl:when>
		<xsl:when test="$cropType=5">Cucumber</xsl:when>
		<xsl:when test="$cropType=6">SweetPepper</xsl:when>
		<xsl:when test="$cropType=7">Lettuce</xsl:when>
		<xsl:when test="$cropType=8">PotPlant</xsl:when>
		<xsl:when test="$cropType=24">Euphorbia</xsl:when>
		<xsl:otherwise>PotPlant</xsl:otherwise>
	</xsl:choose>
</xsl:variable>

<!-- GLOBAL METHODS -->

<xsl:template name="extract-crop-model">
	<xsl:param name="modelName"/>
	<xsl:comment>
		<xsl:value-of select="concat($crop-name, ' ', $modelName)"/>
	</xsl:comment>
	<xsl:copy-of select="$crops/*[name()=$crop-name]/box[@name=$modelName]"/>
</xsl:template>

<xsl:template name="extract-crop-parameters">
	<xsl:copy-of select="$crops/*[name()=$crop-name]/box[@name='crop']//port"/>
</xsl:template>

<xsl:template name="extract-period">
	<xsl:param name="circadian"/>
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
		<xsl:attribute name="value">
			<xsl:value-of select="concat(floor(FromTime div 60), ':', FromTime mod 60)"/>
		</xsl:attribute>
	</port>
	<port name="endTime">
		<xsl:attribute name="value">
			<xsl:value-of select="concat(floor(ToTime div 60), ':', ToTime mod 60)"/>
		</xsl:attribute>
	</port>
	<port name="circadian">
		<xsl:attribute name="value">
			<xsl:value-of select="$circadian"/>
		</xsl:attribute>
	</port>
</xsl:template>

<xsl:template name="cover-position-name">
	<xsl:param name="number"/>
	<xsl:choose>
		<xsl:when test="$number=1">roof1</xsl:when>
		<xsl:when test="$number=2">roof2</xsl:when>
		<xsl:when test="$number=3">side1</xsl:when>
		<xsl:when test="$number=4">side2</xsl:when>
		<xsl:when test="$number=5">end1</xsl:when>
		<xsl:when test="$number=6">end2</xsl:when>
		<xsl:when test="$number=7">floor</xsl:when>
		<xsl:otherwise>unknown</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template name="screen-position-name">
	<xsl:param name="number"/>
	<xsl:choose>
		<xsl:when test="$number=1">roof1_roof2_trapez</xsl:when>
		<xsl:when test="$number=2">roof1_roof2_horizontal</xsl:when>
		<xsl:when test="$number=3">roof1</xsl:when>
		<xsl:when test="$number=4">roof2</xsl:when>
		<xsl:when test="$number=5">side1</xsl:when>
		<xsl:when test="$number=6">side2</xsl:when>
		<xsl:when test="$number=7">end1</xsl:when>
		<xsl:when test="$number=8">end2</xsl:when>
		<xsl:otherwise>unknown</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template name="screen-layer-name">
	<xsl:param name="number"/>
	<xsl:choose>
		<xsl:when test="$number=1">outer</xsl:when>
		<xsl:when test="$number=2">mid</xsl:when>
		<xsl:when test="$number=3">inner</xsl:when>
		<xsl:otherwise>unknown</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template name="screen-type-name">
	<xsl:param name="number"/>
	<xsl:choose>
		<xsl:when test="$number=1">shade</xsl:when>
		<xsl:when test="$number=2">energy</xsl:when>
		<xsl:when test="$number=3">blackout</xsl:when>
		<xsl:otherwise>unknown</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template name="lamp-type">
	<xsl:param name="number"/>
	<xsl:choose>
		<xsl:when test="$number=1">HPSL</xsl:when>
		<xsl:when test="$number=2">LED</xsl:when>
		<xsl:otherwise>unknown</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template name="extract-screen-parameters">
	<xsl:param name="screen-type"/>
	<port name="position">
		<xsl:attribute name="value">
			<xsl:call-template name="screen-position-name">
				<xsl:with-param name="number" select="Position"/>
			</xsl:call-template>
		</xsl:attribute>
	</port>
	<port name="layer">
		<xsl:attribute name="value">
			<xsl:call-template name="screen-layer-name">
				<xsl:with-param name="number" select="Layer"/>
			</xsl:call-template>
		</xsl:attribute>
	</port>
	<port name="emissivityInner">
		<xsl:attribute name="value">
			<xsl:value-of select="$ScreenEmissivityInner"/>
		</xsl:attribute>
	</port>
	<port name="emissivityOuter">
		<xsl:attribute name="value">
			<xsl:value-of select="$ScreenEmissivityOuter"/>
		</xsl:attribute>
	</port>
	<port name="transmissivityLight">
		<xsl:attribute name="value">
			<xsl:value-of select="Constants/Parameters[ParameterID='30']/Value div 100"/>
		</xsl:attribute>
	</port>
	<port name="transmissivityAir">
		<xsl:attribute name="value">
			<xsl:value-of select="Constants/Parameters[ParameterID='32']/Value div 100"/>
		</xsl:attribute>
	</port>
	<port name="energyLossReduction">
		<xsl:attribute name="value">
			<xsl:value-of select="Constants/Parameters[ParameterID='33']/Value div 100"/>
		</xsl:attribute>
	</port>
	
	<port name="U">
		<xsl:attribute name="value">
			<xsl:value-of select="$ScreenU"/>
		</xsl:attribute>
	</port>
	
	<port name="haze">
		<xsl:attribute name="value">
			<xsl:value-of select="$ScreenHaze"/>
		</xsl:attribute>
	</port>
	<port name="state">
		<xsl:attribute name="ref">
			<xsl:value-of select="concat('actuators/screens/', $screen-type, '/control[state]')"/>
		</xsl:attribute>
	</port>
</xsl:template>

<xsl:template name="extract-screens">
	<xsl:param name="shelter-position"/>
	<box name="screens" class="vg::Screens">
		<xsl:for-each select="/JobDataSet/Greenhouse/zone/Screens//Screen">
			<xsl:variable name="screen-type">
				<xsl:call-template name="screen-type-name">
					<xsl:with-param name="number" select="Constants/Parameters[ParameterID='573']/Value"/>
				</xsl:call-template>
			</xsl:variable>
			<xsl:variable name="screen-position">
				<xsl:call-template name="screen-position-name">
					<xsl:with-param name="number" select="Position"/>
				</xsl:call-template>
			</xsl:variable>
			<xsl:if test="contains($screen-position, $shelter-position)">
				<box>
					<xsl:attribute name="name">
						<xsl:value-of select="$screen-type"/>
					</xsl:attribute>
					<xsl:attribute name="class">
						<xsl:value-of select="'vg::Screen'"/>
					</xsl:attribute>
					<xsl:call-template name="extract-screen-parameters">
						<xsl:with-param name="screen-type" select="$screen-type"/>
					</xsl:call-template>
				</box>
			</xsl:if>
		</xsl:for-each>					
	</box>
</xsl:template>

<xsl:template name="extract-shelter">
	<box>
		<xsl:variable name="shelter-position">
			<xsl:call-template name="cover-position-name">
				<xsl:with-param name="number" select="Position"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:attribute name="name">
			<xsl:value-of select="$shelter-position"/>
		</xsl:attribute>
		<xsl:attribute name="class">
			<xsl:value-of select="'vg::Shelter'"/>
		</xsl:attribute>

		<box name="cover" class="vg::Cover">
			<port name="U4">
				<xsl:attribute name="value">
					<xsl:value-of select="Constants/Parameters[ParameterID='310']/Value"/>
				</xsl:attribute>
			</port>
			<port name="transmissivity">
				<xsl:attribute name="value">
					<xsl:value-of select="Constants/Parameters[ParameterID='308']/Value div 100"/>
				</xsl:attribute>
			</port>
			<port name="emissivity">
				<xsl:attribute name="value">
					<xsl:value-of select="$CoverEmissitivity"/>
				</xsl:attribute>
			</port>
			<port name="absorptivity">
				<xsl:attribute name="value">
					<xsl:value-of select="$CoverAbsorptivity"/>
				</xsl:attribute>
			</port>
			<port name="specificHeatCapacity">
				<xsl:attribute name="value">
					<xsl:value-of select="$CoverHeatCapacity"/>
				</xsl:attribute>
			</port>
			<port name="haze">
				<xsl:attribute name="value">
					<xsl:value-of select="$CoverHaze"/>
				</xsl:attribute>
			</port>
		</box> <!-- cover -->

		<xsl:call-template name="extract-screens">
			<xsl:with-param name="shelter-position" select="$shelter-position"/>
		</xsl:call-template>
	</box>
</xsl:template>

<xsl:template name="crop-layer">
	<xsl:param name="layer"/>
	<box name="windSpeed" class="vg::LeafWindSpeed">
		<port name="k" value="0.6"/>
	</box>
	<xsl:call-template name="extract-crop-model">
		<xsl:with-param name="modelName">rs</xsl:with-param>
	</xsl:call-template>
	<box name="rb" class="vg::BoundaryLayerResistanceStanghellini"/>
	<box name="radiationAbsorbed" class="vg::LeafRadiationAbsorbed">
		<port name="lightAbsorbed">
			<xsl:attribute name="label">
				<xsl:value-of select="concat($layer, 'LightAbsorbed')"/>
			</xsl:attribute>
		</port>
		<port name="growthLightLwAbsorbed">
			<xsl:attribute name="label">
				<xsl:value-of select="concat($layer, 'GrowthLightLwAbsorbed')"/>
			</xsl:attribute>
		</port>
		<port name="floorLwAbsorbed">
			<xsl:attribute name="label">
				<xsl:value-of select="concat($layer, 'FloorLwAbsorbed')"/>
			</xsl:attribute>
		</port>
		<port name="shelterLoss">
			<xsl:attribute name="label">
				<xsl:value-of select="concat($layer, 'ShelterLoss')"/>
			</xsl:attribute>
		</port>
		<port name="heatingAbsorbed">
			<xsl:attribute name="label">
				<xsl:value-of select="concat($layer, 'HeatingAbsorbed')"/>
			</xsl:attribute>
		</port>
	</box>
	<box name="transpiration" class="vg::LeafTranspiration"/>
	<!--
	<box name="condensation">
		<box name="vapourFlux" class="vg::VapourFluxCondensation">
			<port name="surfaceAreaPerGroundArea" ref="./lai[value]"/>
			<port name="surfaceTemperature" ref="../../temperature[value]"/>
			<box name="lai" class="vg::LeafLai"/>
		</box>
		<box name="energyFlux" class="vg::EnergyFluxCondensation">
			<port name="vapourFlux" ref="../vapourflux[vapourFlux]"/>
		</box>
	</box>
	-->
	<box name="temperature" class="vg::LeafTemperature">
		<port name="value">
			<xsl:attribute name="label">
				<xsl:value-of select="concat($layer, 'Temperature')"/>
			</xsl:attribute>
		</port>
	</box>
	<box name="photosynthesis" class="vg::LeafPhotosynthesis">
		<box name="lightResponse" class="vg::LeafLightResponse"/>	
	</box>
</xsl:template>
	
	
<!-- MAIN -->
		
<xsl:template match="/"> <box class="Simulation" name="VirtualGreenhouse">
	<xsl:variable name="startTime" select="JobDataSet/StartTime"/>
	<xsl:variable name="stopTime" select="JobDataSet/StopTime"/>
	<xsl:variable name="startYear" select="number( substring($startTime, 1, 4) )"/>
	<xsl:variable name="stopYear" select="number( substring($stopTime, 1, 4) )"/>
	<xsl:variable name="incYear" select="$stopYear - $startYear"/>

	<box name="simulationTimeInterval" class="vg::SimulationTimeInterval">
		
		<port name="beginDate">
			<xsl:attribute name="value">
				<xsl:choose>
					<xsl:when test="$test-mode=0">
						<xsl:value-of select="concat(2001, substring($startTime, 5, 6))"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="$BeginDate"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:attribute>
		</port>

		<port name="endDate">
			<xsl:attribute name="value">
				<xsl:choose>
					<xsl:when test="$test-mode=0">
						<xsl:value-of select="concat(2001 + $incYear, substring($stopTime, 5, 6))"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="$EndDate"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:attribute>
		</port>

		<port name="beginTime">
			<xsl:attribute name="value">
				<xsl:value-of select="substring($startTime, 12, 5)"/>
			</xsl:attribute>
		</port>
		<port name="endTime">
			<xsl:attribute name="value">
				<xsl:value-of select="substring($stopTime, 12, 5)"/>
			</xsl:attribute>
		</port>

		<port name="timeStep" value="5"/>
		<!--
		<port name="timeStep">
			<xsl:attribute name="value">
				<xsl:value-of select="JobDataSet/SimulationTimeStep"/>
			</xsl:attribute>
		</port>
		-->
		<port name="timeUnit" value="m"/>
	</box>

	<port name="steps" ref="simulationTimeInterval[steps]"/>

	<box name="calendar" class="boxes::Calendar">
		<xsl:variable name="simTimeStep" select="JobDataSet/SimulationTimeStep"/>
		<xsl:variable name="outTimeStep" select="JobDataSet/OutputTimeStep"/>
		<port name="initialDateTime" ref="simulationTimeInterval[beginDateTime]"/>
		<port name="timeStep" ref="simulationTimeInterval[timeStep]"/>
		<port name="timeUnit" ref="simulationTimeInterval[timeUnit]"/>
		<port name="sample" value="12"/>
		<port name="latitude">
			<xsl:attribute name="value" select="JobDataSet/Latitude"/>
		</port>
		<port name="longitude">
			<xsl:attribute name="value" select="JobDataSet/Longitude"/>
		</port>
		<port name="timeZone">
			<xsl:attribute name="value">
				<xsl:value-of select="substring($startTime, 21, 2)"/>
			</xsl:attribute>
		</port>
	</box>

	<box name="outdoors" class="vg::Outdoors">
		<port name="co2">
			<xsl:attribute name="value">
				<xsl:value-of select="$OutdoorsCo2"/>
			</xsl:attribute>
		</port>
		<port name="windSpeed" label="outdoorsWindSpeed"/>
		<port name="rh" label="outdoorsRh"/>
		<port name="ah" label="outdoorsAh"/>
		<box name="records" class="boxes::Records">
			<port name="fileName" value=".../input/sel_dk.txt"/>
		</box>
	</box>

	<box name="construction">
		<box name="geometry" class="vg::ConstructionGeometry">
			<port name="spanWidth">
				<xsl:attribute name="value">
					<xsl:value-of select="JobDataSet/Greenhouse/constants/Parameters/Parameters[ParameterID='11']/Value"/>
				</xsl:attribute>
			</port>
			<port name="numSpans">
				<xsl:attribute name="value">
					<xsl:value-of select="JobDataSet/Greenhouse/constants/Parameters/Parameters[ParameterID='9']/Value"/>
				</xsl:attribute>
			</port>
			<port name="height">
				<xsl:attribute name="value">
					<xsl:value-of select="JobDataSet/Greenhouse/constants/Parameters/Parameters[ParameterID='13']/Value"/>
				</xsl:attribute>
			</port>
			<port name="length">
				<xsl:attribute name="value">
					<xsl:value-of select="JobDataSet/Greenhouse/constants/Parameters/Parameters[ParameterID='223']/Value"/>
				</xsl:attribute>
			</port>
			<port name="roofPitch">
				<xsl:attribute name="value">
					<xsl:value-of select="JobDataSet/Greenhouse/constants/Parameters/Parameters[ParameterID='14']/Value"/>
				</xsl:attribute>
			</port>
			<port name="shade">
				<xsl:attribute name="value">
					<xsl:value-of select="JobDataSet/Greenhouse/constants/Parameters/Parameters[ParameterID='445']/Value div 100"/>
				</xsl:attribute>
			</port>
		</box>

		<box name="shelters" class="vg::Shelters">
			<xsl:for-each select="JobDataSet/Greenhouse/zone/Panes//Pane[Position &lt; 7]">
				<xsl:call-template name="extract-shelter"/>
			</xsl:for-each>		
			<!--
			<box name="roof" class="vg::Shelters">
				<xsl:for-each select="JobDataSet/Greenhouse/zone/Panes//Pane[Position &lt; 3]">
					<xsl:call-template name="extract-shelter"/>
				</xsl:for-each>		
			</box>
			<box name="walls" class="vg::Shelters">
				<xsl:for-each select="JobDataSet/Greenhouse/zone/Panes//Pane[Position > 2 and Position &lt; 7]">
					<xsl:call-template name="extract-shelter"/>
				</xsl:for-each>		
			</box>
			-->
		</box>


		<box name="vents" class="vg::Vents">
			<xsl:for-each select="//JobDataSet/Greenhouse/zone/Vents/Vent">
				<box class="vg::Vent">
					<xsl:attribute name="name">
						<xsl:value-of select="concat('vent', position())"/>
					</xsl:attribute>
					<!--<parameter name="maxOpening">
						<xsl:attribute name="value">
							<xsl:value-of select="Constants/Parameters[ParameterID='52']/Value"/>
						</xsl:attribute>
					</parameter>-->
					<port name="length">
						<xsl:attribute name="value">
							<xsl:value-of select="Constants/Parameters[ParameterID='456']/Value"/>
						</xsl:attribute>
					</port>
					<port name="height">
						<xsl:attribute name="value">
							<xsl:value-of select="Constants/Parameters[ParameterID='457']/Value"/>
						</xsl:attribute>
					</port>
					<port name="number">
						<xsl:attribute name="value">
							<xsl:value-of select="Constants/Parameters[ParameterID='49']/Value"/>
						</xsl:attribute>
					</port>
					<port name="porosity">
						<xsl:attribute name="value">
							<xsl:value-of select="Constants/Parameters[ParameterID='51']/Value div 100"/>
						</xsl:attribute>
					</port>
				</box>
			</xsl:for-each>
		</box>
	</box>
	
	<box name="indoors">
		<box name="given">
			<box name="airFlux" class="AirFluxGiven">
				<port name="value" label="givenAirFluxTotal"/>
				<box name="infiltration" class="vg::AirFluxInfiltration">
					<port name="value" label="givenAirFluxInfiltration"/>
					<port name="leakage">
						<xsl:attribute name="value">
							<xsl:value-of select="$Leakage"/>
						</xsl:attribute>
					</port>
				</box>
				
				<box name="crackVentilation" class="vg::PidControlElement">
					<port name="value" label="givenAirFluxCrack"/>
					<port name="signal" ref="./target[signal]"/>
					<port name="Kprop" value="0.1"/>
					<box name="target" class="vg::ProportionalSignal">
						<box name="coldFactor" class="vg::ProportionalSignal"> 
							<port name="input" ref ="outdoors[temperature]"/>
							<port name="threshold" value ="-5"/>    	<!-- sp.VentsspFrostProtection_alpha=-5  -->
							<port name="thresholdBand" value="1"/>
							<port name="increasingSignal" value="TRUE"/>
							<port name="maxSignal">
								<xsl:attribute name="value">
									<xsl:value-of select="$MaxCrackVentilation"/>
								</xsl:attribute>
							</port>
							<port name="minSignal" value="0"/>
						</box>
						
						<port name="input" ref="indoors/humidity[rh]"/>
						<port name="threshold" ref="setpoints/humidity/maximumRh[signal]"/>
						<port name="thresholdBand">
							<xsl:attribute name="value">
								<xsl:value-of select="max(JobDataSet/Greenhouse/zone/Vents/Vent/Constants/Parameters[ParameterID='492']/Value)"/> <!--Hack-->
							</xsl:attribute>
						</port>
						<port name="increasingSignal" value="TRUE"/>
						<port name="maxSignal" ref="./coldFactor[signal]"/>  
						<port name="minSignal" value="0"/>
					</box>
				</box>
				
				<box name="gravitation" class="AirFluxGravitation">
					<port name="value" label="givenAirFluxGravitation"/>
				</box>
			</box> <!-- airFlux -->

			<box name="vapourFlux" class="vg::VapourFluxSum"> 
				<box name="transpiration" class="vg::VapourFluxTranspiration"/>
				<box name="condensationCover" class="vg::VapourFluxCondensation">
					<port name="surfaceAreaPerGroundArea" ref="geometry[coverPerGroundArea]"/>
					<port name="surfaceTemperature" ref="energyFlux/shelter[coverTemperature]"/>
				</box>
				<box name="condensationScreens" class="vg::VapourFluxCondensation">
					<port name="surfaceAreaPerGroundArea" ref="construction/shelters[screensPerGroundArea]"/>
					<port name="surfaceTemperature" ref="energyFlux/shelter[screensTemperature]"/>
				</box>
				<box name="airFluxOutdoors"  class="vg::VapourFluxAir">
					<port name="airFlux" ref="given/airFlux[value]"/>
				</box>
			</box>
			
			<box name="energyFlux" class="vg::EnergyFluxSum"> 
				<box name="light" class="vg::Sum">
					<port name="inputs" value="(indoors/light[total])"/>
				</box>
				<box name="condensationCover" class="vg::EnergyFluxCondensation">
					<port name="vapourFlux" ref="../../vapourFlux/condensationCover[vapourFlux]"/>
				</box>
				<box name="condensationScreens" class="vg::EnergyFluxCondensation">
					<port name="vapourFlux" ref="../../vapourFlux/condensationScreens[vapourFlux]"/>
				</box>
				<box name="airFlux" class="vg::EnergyFluxAir">
					<port name="airFlux" ref="given/airFlux[value]"/>
				</box>
				<box name="growthLights" class="vg::PidControlElement">
					<port name="Kprop" value="0.5"/>
					<port name="signal" ref="./growthLights[value]"/>
					<box name="growthLights" class="vg::EnergyFluxGrowthLights"/>
				</box>
				<box name="shelter" class="vg::EnergyFluxShelters"/>
				<box name="floor" class="vg::EnergyFluxFloor"> 
					<box name="radiationAbsorbed" class="vg::FloorRadiationAbsorbed"/>
					<port name="Uindoors">
						<xsl:attribute name="value">
							<xsl:value-of select="$FloorUindoors"/>
						</xsl:attribute>
					</port>
					<port name="Usoil">
						<xsl:attribute name="value">
							<xsl:value-of select="$FloorUsoil"/>
						</xsl:attribute>
					</port>
					<port name="heatCapacity">
						<xsl:attribute name="value">
							<xsl:value-of select="$FloorHeatCapacity"/>
						</xsl:attribute>
					</port>
					<port name="temperature" label="floorTemperature"/>
				</box>
				
				<box name="transpiration" class="vg::EnergyFluxTranspiration"/> 
			</box>		
		</box> <!-- given -->		

		<box name="controlled">
			<box name="energyFlux"> 
				<box name="heating">
					<box name="minSupply" class="vg::Sum">
						<port name="inputs" value="(actuators/heating/pipes/*[nextEnergyFluxMin])"/>
					</box>
					<box name="maxSupply" class="vg::Sum">
						<port name="inputs" value="(actuators/heating/pipes/*[nextEnergyFluxMax])"/>
					</box>
					<box name="demand" class="vg::EnergyFluxHeatingDemand"/>
					<box name="supply" class="vg::PidControlElement">
						<port name="value" label="heatingSupply"/>
						<port name="Kprop" value="0.6"/>
						<port name="Kint" value="0.01"/>
						<port name="minimum" ref="../minSupply[value]"/>
						<port name="maximum" ref="../maxSupply[value]"/>
						<port name="signal" ref="../demand[value]"/>
					</box>				
				</box>		
				
				<box name="temperature" class="vg::IndoorsTemperature">
					<box name="energyFlux" class="vg::Sum">
						<port name="inputs" value="(given/energyFlux[value] energyFlux/heating/supply[value])"/>
					</box>
					<port name="energyFlux" ref="./energyFlux[value]"/>
					<port name="baseTemperature" ref="indoors/temperature[value]"/>
				</box>

				<box name="cooling"> 
					<box name="demand" class="vg::EnergyFluxCoolingDemand">
						<port name="value" label="energyFluxCoolingDemand"/>
					</box>
					<box name="airSupplyMax" class="vg::AirFluxCoolingSupplyMax">
						<port name="value" label="airFluxSupplyMaxTotal"/>
						<box name="byWind" class="vg::VentilationByWind">
							<port name="value" label="airFluxSupplyMaxWind"/>
							<port name="baseRate" value="10"/>
						</box>
						<box name="byTemp" class="vg::VentilationByTemp">
							<port name="value" label="airFluxSupplyMaxThermal"/>
							<port name="dischargeCoefficient" value="0.3"/>
						</box>
					</box>
					<box name="supply" class="vg::PidControlElement">
						<port name="value" label="energyFluxCoolingSupply"/>
						<port name="slope" label="energyFluxCoolingSupplySlope"/>
						<port name="Kprop" value="0.5"/>
						<port name="maximum" value="0"/>
						<port name="maxSlope" value="1"/>
						<port name="minSlope" value="-1"/>
						<port name="signal" ref="./target[value]"/>
						<box name="target" class="vg::EnergyFluxCoolingSupply">
							<port name="airSupplyMax" ref="cooling/airSupplyMax[value]"/>
						</box>
					</box>
				</box>
			</box> <!-- energyFlux -->

			<box name="cooling">
				<box name="airFlux" class="vg::AirFluxCoolingSupply">
					<port name="value" label="airFluxSupply"/>
				</box>
				<box name="vapourFlux" class="vg::VapourFluxAir">
					<port name="airFlux" ref="cooling/airFlux[value]"/>
				</box>
				<box name="energyFlux" class="vg::EnergyFluxAir">
					<port name="airFlux" ref="cooling/airFlux[value]"/>
				</box>
			</box>
		</box> <!-- controlled -->
		
		<box name="total">
			<box name="airFlux" class="vg::Sum">
				<port name="value" label="airFluxTotal"/>
				<port name="inputs" value="(given/airFlux[value] cooling/airFlux[value])"/>
			</box>
			<box name="vapourFlux" class="vg::VapourFluxSum">
				<port name="toAdd" value="(given/vapourFlux cooling/vapourFlux)"/>
			</box>
			<box name="energyFlux" class="vg::Sum">
				<port name="inputs" value="(given/energyFlux[value] energyFlux/heating/supply[value] cooling/energyFlux[value])"/>
			</box>
		</box> <!-- total -->

		<box name="light" class="vg::IndoorsLight"/> 
		<box name="temperature" class="vg::IndoorsTemperature">
			<port name="resetValue">
				<xsl:attribute name="value">
					<xsl:value-of select="//Setpoint[ParameterId='2']//SetpointValue"/>
				</xsl:attribute>
			</port> 
			<port name="energyFlux" ref="total/energyFlux[value]"/>
			<port name="value" label="indoorsTemperature"/>
		</box>
		<box name="humidity" class="vg::IndoorsHumidity">
			<port name="rh" label="indoorsRh"/>
			<port name="ah" label="indoorsAh"/>
		</box>
		<box name="co2" class="vg::IndoorsCo2">
			<port name="value" label="indoorsCo2"/>
		</box>
		<box name="windSpeed" class="vg::IndoorsWindSpeed">
			<port name="value" label="indoorsWindSpeed"/>
		</box>
	</box> <!-- indoors -->

	<box name="setpoints">
		<box name="isDay" class="vg::ProportionalSignal">
			<port name="threshold" >
				<xsl:attribute name="value">
					<xsl:value-of select="//Greenhouse/zone/Constants/Parameters[ParameterID='436']/Value"/>
				</xsl:attribute>
			</port>
			<port name="input" ref="outdoors/records[GlobRad]"/>
			<port name="thresholdBand" value="0"/>
			<port name="increasingSignal" value="TRUE"/>
			<port name="minSignal" value="0"/>
			<port name="maxSignal" value="1"/>
		</box>
		<box name="humidity">
			<!-- Day/Night setpoints not implemented in MatLab nor UniSim model -->
			<!-- DeltaX setpoint not implemented in UniSim model -->
			<box name="maximumRh" class="vg::SignalCollection">
				<port name="signal" label="setpointMaxRh"/>
				<port name="rule" value="min"/>
				<port name="signalReset" value="100"/>
				<xsl:for-each select="//CultureStep/Setpoints/Setpoint[ParameterId='16']/SetpointTimes//SetpointTime">
					<box class="vg::DateTimeSignal">
						<port name="signalOutside" value="100"/>
						<port name="signalOutsideTimeOnly" value="100"/>
						<port name="signalInside">
							<xsl:attribute name="value">
								<xsl:value-of select="SetpointValue"/>
							</xsl:attribute>
						</port>
						<xsl:call-template name="extract-period">
							<xsl:with-param name="circadian" select="'TRUE'"/>
						</xsl:call-template>
					</box>
				</xsl:for-each>
			</box>
		</box>
		
		<box name="temperature">
			<box name="ventilation" class="vg::PidControlElement">
				<port name="value" label="setpointVentilation"/>
				<port name="initState" ref="./target[signalReset]"/>
				<port name="signal" ref="./target[signal]"/>
				<port name="Kprop" value="0.1"/>
				<box name="target" class="vg::SignalCollection">
					<port name="rule" value="sum"/>
					<port name="signalReset">
						<xsl:attribute name="value">
							<xsl:value-of select="//Setpoint[ParameterId='3']//SetpointValue"/>
						</xsl:attribute>
					</port> 

					<box name="setpoints" class="vg::SignalCollection">
						<port name="rule" value="min"/>
						<port name="signalReset" ref="..[signalReset]"/>
						<xsl:for-each select="//CultureStep/Setpoints/Setpoint[ParameterId='3']/SetpointTimes//SetpointTime">
							<box name="setpoint" class="vg::DateTimeSignal">
								<xsl:call-template name="extract-period">
									<xsl:with-param name="circadian" select="'TRUE'"/>
								</xsl:call-template>
								<port name="signalOutside" value="100"/>
								<port name="signalOutsideTimeOnly" value="100"/>
								<port name="signalInside">
									<xsl:attribute name="value">
										<xsl:value-of select="SetpointValue"/>
									</xsl:attribute>
								</port>
							</box>
						</xsl:for-each>
					</box>
					
					<!-- Decrease ventilation set point temperature at high humidity -->
					<box name="adjustments" class="vg::SignalCollection">
						<port name="rule" value="min"/>
						<port name="signalReset" value="0"/>
						<xsl:for-each select="//CultureStep/Setpoints/Setpoint[ParameterId='40']/SetpointTimes//SetpointTime">

							<box class="vg::DateTimeSignal">
								<xsl:call-template name="extract-period">
									<xsl:with-param name="circadian" select="'TRUE'"/>
								</xsl:call-template>
								<port name="signalOutside" value="0"/>
								<port name="signalOutsideTimeOnly" value="0"/>
								<port name="signalInside" ref="./adjustment[signal]"/>
					
								<box name="adjustment" class="vg::ProportionalSignal">
									<port name="input" ref="indoors/humidity[rh]"/>
									<port name="threshold" ref="setpoints/humidity/maximumRh[signal]"/>
									<port name="thresholdBand">
										<xsl:variable name="user-sp" select="max(//Vents/Vent/Constants/Parameters[ParameterID='489']/Value)"/>
										<xsl:attribute name="value">
											<xsl:if test="$user-sp &lt; $MinVentilationPbandRH">
												<xsl:value-of select="$MinVentilationPbandRH"/>
											</xsl:if>
											<xsl:if test="$user-sp &gt;= $MinVentilationPbandRH">
												<xsl:value-of select="$user-sp"/>
											</xsl:if>
										</xsl:attribute>
									</port>
									<port name="increasingSignal" value="FALSE"/>
									<port name="minSignal">
										<xsl:attribute name="value">
											<xsl:value-of select="-SetpointValue"/>
										</xsl:attribute>
									</port>
									<port name="maxSignal" value="0"/>
								</box>
							</box>
						</xsl:for-each>
					</box>
				</box>
			</box>
		
			<!-- Day/Night setpoints not implemented in MatLab nor UniSim model -->
			<!-- spTheatmax to avoid cold-fall from screens not implemented in MatLab nor UniSim model-->
			<box name="heating" class="vg::PidControlElement">
				<port name="value" label="setpointHeating"/>
				<port name="initState" ref="./target[signalReset]"/>
				<port name="signal" ref="./target[signal]"/>
				<port name="Kprop" value="0.1"/>
				<box name="target" class="vg::SignalCollection">
					<port name="rule" value="sum"/>
					<port name="signalReset">
						<xsl:attribute name="value">
							<xsl:value-of select="//Setpoint[ParameterId='2']//SetpointValue"/>
						</xsl:attribute>
					</port> 
					<box name="setpoints" class="vg::SignalCollection">
						<port name="rule" value="max"/>
						<port name="signalReset" ref="..[signalReset]"/>
						<xsl:for-each select="//CultureStep/Setpoints/Setpoint[ParameterId='2']/SetpointTimes//SetpointTime">
						
							<box name="setpoint" class="vg::DateTimeSignal">
								<xsl:call-template name="extract-period">
									<xsl:with-param name="circadian" select="'TRUE'"/>
								</xsl:call-template>
								<port name="signalOutside" value="0"/>
								<port name="signalOutsideTimeOnly" value="0"/>
								<port name="signalInside">
									<xsl:attribute name="value">
										<xsl:value-of select="SetpointValue"/>
									</xsl:attribute>
								</port>
							</box>
						</xsl:for-each>
					</box>
					
					<!-- Increase heating set point at high humidity -->
					<box name="adjustmentsRh" class="vg::SignalCollection">
						<port name="signalReset" value="0"/>
						<port name="rule" value="max"/>
						<xsl:for-each select="//CultureStep/Setpoints/Setpoint[ParameterId='22']/SetpointTimes//SetpointTime">
						
							<box class="vg::DateTimeSignal">
								<xsl:call-template name="extract-period">
									<xsl:with-param name="circadian" select="'TRUE'"/>
								</xsl:call-template>
								<port name="signalReset" value="0"/>
								<port name="signalOutside" value="0"/>
								<port name="signalOutsideTimeOnly" value="0"/>
								<port name="signalInside" ref="./adjustment[signal]"/>
					
								<box name="adjustment" class="vg::ProportionalSignal">
									<port name="input" ref="indoors/humidity[rh]"/>
									<port name="threshold" ref="setpoints/humidity/maximumRh[signal]"/>
									<port name="thresholdBand">
										<xsl:variable name="user-sp" select="max(//Heatpipes/Heatpipe/Constants/Parameters[ParameterID='540']/Value)"/>
										<xsl:attribute name="value">
											<xsl:if test="$user-sp &lt; $MinHeatingPbandRH">
												<xsl:value-of select="$MinHeatingPbandRH"/>
											</xsl:if>
											<xsl:if test="$user-sp &gt;= $MinHeatingPbandRH">
												<xsl:value-of select="$user-sp"/>
											</xsl:if>
										</xsl:attribute>
									</port>
									<port name="increasingSignal" value="TRUE"/>
									<port name="minSignal" value="0"/>
									<port name="maxSignal">
										<xsl:attribute name="value">
											<xsl:value-of select="SetpointValue"/>
										</xsl:attribute>
									</port>
								</box>
							</box>
						</xsl:for-each>
					</box>

					<!-- Increase heating set point at high light -->
					<box name="adjustmentLight" class="vg::ProportionalSignal">
						<port name="input" ref="outdoors[radiation]"/>
						<port name="threshold">
							<xsl:attribute name="value">
								<xsl:value-of select="$HeatingLightAdjustmentThreshold"/>
							</xsl:attribute>
						</port>
						<port name="thresholdBand">
							<xsl:attribute name="value">
								<xsl:value-of select="$HeatingLightAdjustmentThresholdBand"/>
							</xsl:attribute>
						</port>
						<port name="increasingSignal" value="TRUE"/>
						<port name="minSignal" value="0"/>
						<port name="maxSignal">
							<xsl:attribute name="value">
								<xsl:value-of select="$HeatingLightAdjustment"/>
							</xsl:attribute>
						</port>
					</box>
				</box>
			</box>
		</box>

		<box name="co2">
			<box name="minimum" class="vg::SignalCollection">
				<port name="rule" value="max"/>
				<port name="signalReset" value="0"/>
				<port name="rule" value="max"/>
				<xsl:for-each select="//CultureStep/Setpoints/Setpoint[ParameterId='28']/SetpointTimes//SetpointTime">
					<box class="vg::DateTimeSignal">
						<xsl:call-template name="extract-period">
							<xsl:with-param name="circadian" select="'TRUE'"/>
						</xsl:call-template>
						<port name="signalOutside" value="0"/>
						<port name="signalOutsideTimeOnly" value="0"/>
						<port name="signalInside">
							<xsl:attribute name="value">
								<xsl:value-of select="SetpointValue"/>
							</xsl:attribute>
						</port>
					</box>
				</xsl:for-each>
			</box>

			<box name="maximum" class="vg::SignalCollection">
				<port name="rule" value="min"/>
				<port name="signalReset" value="1500"/>
				<xsl:for-each select="//CultureStep/Setpoints/Setpoint[ParameterId='27']/SetpointTimes//SetpointTime">
					<box class="vg::DateTimeSignal">
						<xsl:call-template name="extract-period">
							<xsl:with-param name="circadian" select="'TRUE'"/>
						</xsl:call-template>
						<port name="signalOutside" value="0"/>
						<port name="signalOutsideTimeOnly" value="0"/>
						<port name="signalInside">
							<xsl:attribute name="value">
								<xsl:value-of select="SetpointValue"/>
							</xsl:attribute>
						</port>
					</box>
				</xsl:for-each>
			</box>
		</box>
	</box>

	<box name="controllers">
		<box name="screens">
			<!-- When too humid, do not draw screens completely -->
			<box name="maxDrawn" class="vg::PidControlElement"> 
				<port name="initState" value="1"/>
				<port name="signal" ref="./target[signal]"/>
				<port name="Kprop" value="0.1"/>
				<box name="target" class="vg::ThresholdSignal"> 
					<port name="input" ref ="indoors/humidity[rh]"/>
					<port name="threshold" ref ="setpoints/humidity/maximumRh[signal]"/>
					<port name="signalBelow" value ="1"/>
					<port name="signalAbove">
						<xsl:attribute name="value">
							<xsl:value-of select="1 - max(//Screens/Screen/Constants/Parameters[ParameterID='522']/Value) div 100"/>
						</xsl:attribute>
					</port>
				</box>
			</box>

			<box name="energy">
				<xsl:choose>
					<xsl:when test="$EnergyScreenOption=1">
						<box name="control" class="vg::SignalCollection">
							<port name="rule" value="min"/>
							<port name="signalReset" value ="0"/>
							<!-- Draw when it is dark -->
							<box class="vg::ThresholdSignal">
								<port name="input" ref ="outdoors[radiation]"/>
								<port name="threshold">
									<xsl:attribute name="value">
										<xsl:value-of select="$spLightEnergyScreen"/>
									</xsl:attribute>
								</port>
								<port name="signalBelow" ref ="controllers/screens/maxDrawn[value]"/>
								<port name="signalAbove" value ="0"/>
							</box>
							<!-- And it is cool inside -->
							<box class="vg::ThresholdSignal">
								<port name="input" ref ="indoors/temperature[value]"/>
								<port name="threshold" ref ="setpoints/temperature/ventilation[value]"/>
								<port name="signalBelow" value ="1"/>
								<port name="signalAbove" value ="0"/>
							</box>
							<!-- And the energy balance without screens is negative -->
							<box class="vg::EnergyScreenBalanceSignal"/>
						</box>
					</xsl:when>
					<xsl:when test="$EnergyScreenOption=2">
						<box name="control" class="vg::SignalCollection">
							<port name="rule" value="min"/>
							<port name="signalReset" value ="0"/>
							<!-- Draw when it is dark -->
							<box class="vg::ThresholdSignal">
								<port name="input" ref ="outdoors[radiation]"/>
								<port name="threshold">
									<xsl:attribute name="value">
										<xsl:value-of select="$spLightEnergyScreen"/>
									</xsl:attribute>
								</port>
								<port name="signalBelow" ref ="controllers/screens/maxDrawn[value]"/>
								<port name="signalAbove" value ="0"/>
							</box>
							<!-- And it is cool inside -->
							<box class="vg::ThresholdSignal">
								<port name="input" ref ="indoors/temperature[value]"/>
								<port name="threshold" ref ="setpoints/temperature/ventilation[signal]"/>
								<port name="signalBelow" value ="1"/>
								<port name="signalAbove" value ="0"/>
							</box>
						</box>
					</xsl:when>
				</xsl:choose>
			</box>
			
			<box name="shade">
				<box name="control" class="vg::SignalCollection">
					<port name="rule" value="max"/>
					<port name="signalReset" value ="0"/>
					<!-- Draw when it is too light  -->
					<box class="vg::ThresholdSignal">
						<port name="input" ref ="outdoors[radiation]"/>
						<port name="threshold">
							<xsl:attribute name="value">
								<xsl:value-of select="$spShadowThresholdScreen"/>
							</xsl:attribute>
						</port>
						<port name="signalBelow" value ="0"/>
						<port name="signalAbove" ref ="controllers/screens/maxDrawn[value]"/>
					</box>
					<!-- Or when energy screen is drawn -->
					<xsl:if test="$Use2ndScreenWithEnergy=1">
						<box class="vg::FixedSignal">
							<port name="input" ref="controllers/screens/energy/control[signal]"/>
						</box>
					</xsl:if>
				</box>
			</box>

			<box name="blackout">
				<!-- Draw if there is any light  -->
				<box name="light" class="vg::ThresholdSignal">
					<port name="input" ref="outdoors[radiation]"/>
					<port name="threshold" value="0"/>
					<port name="signalBelow" value="0"/>
					<port name="signalAbove" ref ="controllers/screens/maxDrawn[value]"/>
				</box>
				<box name="control" class="vg::SignalCollection">
					<port name="rule" value="max"/>
					<port name="signalReset" value="0"/>
					<box class="vg::SignalCollection">
						<port name="rule" value="max"/>
						<port name="signalReset" value="0"/>
						<!-- Draw at night time  -->
						<box class="vg::DateTimeSignal">
							<port name="beginDay" value="1"/>
							<port name="endDay" value="365"/>
							<port name="beginTime" value="18:00"/>
							<port name="endTime" value="6:00"/>
							<port name="signalInside" value="1"/>
							<port name="signalOutside" value="0"/>
							<port name="signalOutsideTimeOnly" value="0"/>
						</box>
					</box>
					<!-- Or when energy screen is drawn -->
					<xsl:if test="$Use2ndScreenWithEnergy=1">
						<box class="vg::FixedSignal">
							<port name="input" ref="controllers/screens/energy/control[signal]"/>
						</box>
					</xsl:if>
				</box>
			</box>
		</box>
<!--
		<box name="ventilation" class="vg::VentilationController">
			<box name="byTemperature" class="vg::ProportionalSignal">
				<port name="input" ref="indoors/temperature[value]"/>
				<port name="threshold" ref="setpoints/temperature/ventilation[value]"/>
				<port name="thresholdBand">
					<xsl:attribute name="value">
						<xsl:value-of select="max(JobDataSet/Greenhouse/zone/Vents/Vent/Constants/Parameters[ParameterID='489']/Value)"/>  <-Hack
					</xsl:attribute>
				</port>
				<port name="increasingSignal" value="TRUE"/>
				<port name="maxSignal" value="100"/>
				<port name="minSignal" value="0"/>
			</box>
			<box name="byHumidity" class="vg::ProportionalSignal">
				<port name="input" ref="indoors/humidity[rh]"/>
				<port name="threshold" ref="setpoints/humidity/maximumRh[signal]"/>
				<port name="thresholdBand">
					<xsl:attribute name="value">
						<xsl:value-of select="max(JobDataSet/Greenhouse/zone/Vents/Vent/Constants/Parameters[ParameterID='492']/Value)"/> <-Hack
					</xsl:attribute>
				</port>
				<port name="increasingSignal" value="TRUE"/>
				<port name="maxSignal" value="5"/>  <- sp.spRH.maxVentopeningRH_day=5;  sp.spRH.maxVentopeningRH_night=5; 
				<port name="minSignal" value="0"/>
			</box>
			<box name="leeFactor" class="vg::ProportionalSignal">
				<port name="input" ref="outdoors[windspeed]"/>
				<port name="threshold" value="15"/>
				<port name="thresholdBand" value="5"/>
				<port name="increasingSignal" value="FALSE"/>
				<port name="maxSignal" value="1"/>
				<port name="minSignal" value="0"/>
			</box>
			<box name="windFactor" class="vg::ProportionalSignal">
				<port name="input" ref="outdoors[windspeed]"/>
				<port name="threshold" value="10"/>
				<port name="thresholdBand" value="5"/>
				<port name="increasingSignal" value="FALSE"/>
				<port name="maxSignal" value="1"/>
				<port name="minSignal" value="0"/>
			</box>
			<box name="coldFactor" class="vg::ProportionalSignal"> 
				<port name="input" ref ="outdoors[temperature]"/>
				<port name="threshold" value ="-5"/>    <- sp.VentsspFrostProtection_alpha=-5 
				<port name="thresholdBand" value="1"/>
				<port name="increasingSignal" value="TRUE"/>
				<port name="maxSignal" value="1"/>
				<port name="minSignal" value="0"/>
			</box>
		</box>

		<box name="heating">
			<box name="temperature" class="vg::HeatingTemperatureController">
				<box name="Tdiff" class="vg::RunningAverage">
					<port name="input" ref ="..[Tdiff]"/>
					<port name="length" value ="30"/>
				</box>
				<- For hvert rør:
				Se CommonFlowTemperature og CommonReturnTemperature
				
				<port name="minimumSignal" value ="20"/>
				<port name="maximumSignal" value ="90"/>
			</box>
		</box>
-->
		<box name="growthLight" class="vg::GrowthLightController">
			<box name="on" class="vg::SignalCollection">
				<port name="rule" value ="max"/>
				<port name="signalReset" value ="0"/>
				<xsl:for-each select="//Setpoint[ParameterId='33']//SetpointTime">
					<box class="vg::DateTimeSignal">
						<xsl:call-template name="extract-period">
							<xsl:with-param name="circadian" select="'TRUE'"/>
						</xsl:call-template>
						<port name="signalInside" ref ="./on[signal]"/>
						<port name="signalOutside" value="0"/>
						<port name="signalOutsideTimeOnly" value="0"/>

						<box name="on" class="vg::ThresholdSignal">
							<port name="threshold">
								<xsl:attribute name="value">
									<xsl:value-of select="SetpointValue"/>
								</xsl:attribute>
							</port>
							<port name="input" ref ="outdoors[radiation]"/>
							<port name="signalBelow" value ="1"/>
							<port name="signalAbove" value="0"/>
						</box>
					</box>
				</xsl:for-each>
			</box>
			
			<box name="off" class="vg::SignalCollection">
				<port name="rule" value ="max"/>
				<port name="signalReset" value ="0"/>
				<xsl:for-each select="//Setpoint[ParameterId='34']//SetpointTime">
					<box class="vg::DateTimeSignal">
						<xsl:call-template name="extract-period">
							<xsl:with-param name="circadian" select="'TRUE'"/>
						</xsl:call-template>
						<port name="signalInside" ref ="./off[signal]"/>
						<port name="signalOutside" value="0"/>
						<port name="signalOutsideTimeOnly" value="0"/>

						<box name="off" class="vg::ThresholdSignal">
							<port name="threshold">
								<xsl:attribute name="value">
									<xsl:value-of select="SetpointValue"/>
								</xsl:attribute>
							</port>
							<port name="input" ref ="outdoors[radiation]"/>
							<port name="signalBelow" value ="0"/>
							<port name="signalAbove" value="1"/>
						</box>
					</box>
				</xsl:for-each>
			</box>
			
			<box name="periods" class="vg::SignalCollection">
				<port name="rule" value ="max"/>
				<port name="signalReset" value ="0"/>
				<xsl:for-each select="//Setpoint[ParameterId='35']//SetpointTime[SetpointValue='1']">
					<box class="vg::DateTimeSignal">
						<xsl:call-template name="extract-period">
							<xsl:with-param name="circadian" select="'TRUE'"/>
						</xsl:call-template>
						<port name="signalInside" value ="1"/>
						<port name="signalOutside" value="0"/>
						<port name="signalOutsideTimeOnly" value="0"/>
					</box>
				</xsl:for-each>
			</box>
		</box>

		<box name="chalk" class="vg::SignalCollection">
			<port name="rule" value ="max"/>
			<port name="signalReset" value ="0"/>
			<xsl:for-each select="//CultureStep/Setpoints/Setpoint[ParameterId='39']/SetpointTimes//SetpointTime">
				<box class="vg::DateTimeSignal">
						<xsl:call-template name="extract-period">
							<xsl:with-param name="circadian" select="'FALSE'"/>
						</xsl:call-template>
					<port name="signalOutside" value="0"/>
					<port name="signalInside">
						<xsl:attribute name="value">
							<xsl:value-of select="SetpointValue"/>
						</xsl:attribute>
					</port>
				</box>
			</xsl:for-each>
		</box>
		
		<box name="co2" class="vg::Co2Controller">
			<port name="injectionRate" ref="./injectionRate[signal]"/>
			<box name="injectionRate" class="vg::ProportionalSignal">
				<port name="input" ref="indoors/total/airFlux[value]"/>
				<port name="threshold">
					<xsl:attribute name="value">
						<xsl:value-of select="$Co2VentilationThreshold"/>
					</xsl:attribute>
				</port>
				<port name="thresholdBand">
					<xsl:attribute name="value">
						<xsl:value-of select="$Co2VentilationThresholdBand"/>
					</xsl:attribute>
				</port>
				<port name="increasingSignal" value="FALSE"/>
				<port name="minSignal" value="0"/>
				<port name="maxSignal">
					<xsl:attribute name="value">
						<xsl:value-of select="$Co2MaxSupply"/>
					</xsl:attribute>
				</port>
			</box>
	</box>
	</box>

	<box name="actuators">
		<box name="screens">
			<box name="energy">
				<box name="control" class="vg::PidControlElement">
					<port name="signal" ref="controllers/screens/energy/control[signal]"/>
					<port name="Kprop" value="0.3"/>
				</box>
			</box>
			<box name="shade">
				<box name="control" class="vg::PidControlElement">
					<port name="signal" ref="controllers/screens/shade/control[signal]"/>
					<port name="Kprop" value="0.3"/>
				</box>
			</box>
			<box name="blackout">
				<box name="control" class="vg::PidControlElement">
					<port name="signal" ref="controllers/screens/blackout/control[signal]"/>
					<port name="Kprop" value="0.3"/>
				</box>
			</box>
		</box>
		<!--
		<box name="vents"> 
			<box name="leeside" class="vg::LinearControlElement">
				<port name="signal" ref="controllers/ventilation[leewardSignal]"/>
				<port name="rate" value="10"/>
			</box>
			<box name="windside" class="vg::LinearControlElement">
				<port name="signal" ref="controllers/ventilation[windwardSignal]"/>
				<port name="rate" value="10"/>
			</box>
		</box>
		-->
		<box name="heating">
			<box name="pipes"> 
				<xsl:for-each select="JobDataSet/Greenhouse/zone/Heatpipes/Heatpipe">
					<box>
						<xsl:attribute name="name">
							<xsl:value-of select="concat('pipe', position())"/>
						</xsl:attribute>
						<xsl:attribute name="class">
							<xsl:value-of select="'vg::Pipe'"/>
						</xsl:attribute>
						<port name="length">
							<xsl:attribute name="value">
								<xsl:value-of select="Constants/Parameters[ParameterID='535']/Value"/>
							</xsl:attribute>
						</port>
						<port name="diameter">
							<xsl:attribute name="value">
								<xsl:value-of select="Constants/Parameters[ParameterID='62']/Value*1000"/>
							</xsl:attribute>
						</port>
						<port name="minTemperature">
							<xsl:attribute name="value">
								<xsl:value-of select="Constants/Parameters[ParameterID='359']/Value"/>
							</xsl:attribute>
						</port>
						<port name="maxTemperature">
							<xsl:attribute name="value">
								<xsl:value-of select="Constants/Parameters[ParameterID='358']/Value"/>
							</xsl:attribute>
						</port>
					</box>
				</xsl:for-each>
			</box>
		</box>
		<box name="growthLights" class="vg::GrowthLights">
			<xsl:for-each select="JobDataSet/Greenhouse/zone/Lamps/Lamp">
				<box class="vg::GrowthLight">
					<xsl:attribute name="name">
						<xsl:value-of select="concat('growthLight', position())"/>
					</xsl:attribute>
					<port name="type">
						<xsl:attribute name="value">
							<xsl:call-template name="lamp-type">
								<xsl:with-param name="number" select="Constants/Parameters[ParameterID='575']/Value"/>
							</xsl:call-template>
						</xsl:attribute>
					</port>
					<port name="intensity">
						<xsl:attribute name="value">
							<xsl:value-of select="Constants/Parameters[ParameterID='533']/Value"/>
						</xsl:attribute>
					</port>
					<port name="lifeTime">
						<xsl:attribute name="value">
							<xsl:value-of select="Constants/Parameters[ParameterID='397']/Value"/>
						</xsl:attribute>
					</port>
					<port name="age">
						<xsl:attribute name="value">
							<xsl:value-of select="Constants/Parameters[ParameterID='398']/Value"/>
						</xsl:attribute>
					</port>
				</box>
			</xsl:for-each>
		</box>
	</box>

	<xsl:comment>
		<xsl:value-of select="concat($crop-name, ' model')"/>
	</xsl:comment>

	<box name="crop" class="vg::Crop">
		<xsl:call-template name="extract-crop-parameters"/>

		<xsl:call-template name="extract-crop-model">
			<xsl:with-param name="modelName">periods</xsl:with-param>
		</xsl:call-template>

		<box name="physTime" class="vg::DayDegrees">
			<port name="T" ref="indoors/temperature[value]"/>
			<port name="isTicking" ref="../periods[flag]"/>
			<port name="doReset" ref="../periods[flagDown]"/>
		</box>

		<xsl:call-template name="extract-crop-model">
			<xsl:with-param name="modelName">lai</xsl:with-param>
		</xsl:call-template>

		<box name="layers">
			<box name="top" class="vg::LeafLayer">
				<port name="xGaussUpperside"  value="0.1127"/>
				<port name="wGaussUpperside"  value="0.2778"/>
				<port name="xGaussLowerside"  value="0.8873"/>
				<port name="wGaussLowerside"  value="0.2778"/>
				<xsl:call-template name="crop-layer">
					<xsl:with-param name="layer">top</xsl:with-param>
				</xsl:call-template>
			</box>
			<box name="middle" class="vg::LeafLayer">
				<port name="xGaussUpperside"  value="0.5"/>
				<port name="wGaussUpperside"  value="0.4444"/>
				<port name="xGaussLowerside"  value="0.5"/>
				<port name="wGaussLowerside"  value="0.4444"/>
				<xsl:call-template name="crop-layer">
					<xsl:with-param name="layer">middle</xsl:with-param>
				</xsl:call-template>
			</box>
			<box name="bottom" class="vg::LeafLayer">
				<port name="xGaussUpperside"  value="0.8873"/>
				<port name="wGaussUpperside"  value="0.2778"/>
				<port name="xGaussLowerside"  value="0.1127"/>
				<port name="wGaussLowerside"  value="0.2778"/>
				<xsl:call-template name="crop-layer">
					<xsl:with-param name="layer">bottom</xsl:with-param>
				</xsl:call-template>
			</box>
		</box>
		
		<box name="radiation" class="vg::CropRadiation"/>

		<box name="temperature" class="vg::Average">
			<port name="inputs"  value="(layers/top/temperature[value] layers/middle/temperature[value] layers/bottom/temperature[value])"/> 
		</box>	

		<box name="lightAbsorbed" class="vg::Sum">
			<port name="inputs" value="(layers/top/radiationAbsorbed[lightAbsorbed] layers/middle/radiationAbsorbed[lightAbsorbed] layers/bottom/radiationAbsorbed[lightAbsorbed])"/>
		</box>

		<box name="heatingAbsorbed" class="vg::Sum">
			<port name="inputs" value="(layers/top/radiationAbsorbed[heatingAbsorbed] layers/middle/radiationAbsorbed[heatingAbsorbed] layers/bottom/radiationAbsorbed[heatingAbsorbed])"/>
		</box>

		<box name="growthLightLwAbsorbed" class="vg::Sum">
			<port name="inputs" value="(layers/top/radiationAbsorbed[growthLightLwAbsorbed] layers/middle/radiationAbsorbed[growthLightLwAbsorbed] layers/bottom/radiationAbsorbed[growthLightLwAbsorbed])"/>
		</box>

		<box name="radiationAbsorbed" class="vg::Sum">
			<port name="inputs" value="(layers/top/radiationAbsorbed[lightAbsorbed] layers/middle/radiationAbsorbed[lightAbsorbed] layers/bottom/radiationAbsorbed[lightAbsorbed])"/>
		</box>

		<box name="conductance" class="vg::Average">
			<port name="inputs"  value="(layers/top/transpiration[conductance] layers/middle/transpiration[conductance] layers/bottom/transpiration[conductance])"/> 
		</box>	

		<box name="vapourFlux" class="vg::Average">
			<port name="inputs"  value="(layers/top/transpiration[vapourFlux] layers/middle/transpiration[vapourFlux] layers/bottom/transpiration[vapourFlux])"/>
		</box>
		<box name="gain" class="vg::Average">
			<port name="inputs"  value="(layers/top/transpiration[gain] layers/middle/transpiration[gain] layers/bottom/transpiration[gain])"/>
		</box>

		<box name="growth" class="vg::CropGrowth">
			<box name="Pg" class="vg::Sum">
				<port name="value" label="assimilationRate"/>
				<port name="inputs"  value="(layers/top/photosynthesis[Pg] layers/middle/photosynthesis[Pg] layers/bottom/photosynthesis[Pg])"/>
			</box>
		</box>
		<xsl:call-template name="extract-crop-model">
			<xsl:with-param name="modelName">mass</xsl:with-param>
		</xsl:call-template>
		<xsl:call-template name="extract-crop-model">
			<xsl:with-param name="modelName">yield</xsl:with-param>
		</xsl:call-template>

	</box> <!-- crop -->

	<box name="budget" class="vg::Budget"/>
	<box class="OutputR">
		<port name="xAxis" ref="calendar[dateTime]"/>
		<port name="layout" value="facetted"/>
		<box name="climate" class="PageR">
			<port name="ncol" value="5"/>
			<box name="co2" class="PlotR">
				<port name="ports" value="(outdoors[windSpeed] indoors/windSpeed[value] indoors/co2[value])"/>
			</box>
			<box name="humidity" class="PlotR">
				<port name="ports" value="(outdoors[rh] indoors/humidity[rh] outdoors[ah] indoors/humidity[ah])"/>
			</box>
			<box name="temperature" class="PlotR">
				<port name="ports" value="(indoors/temperature[value] 
											given/energyFlux/shelter[coverTemperature] 
											given/energyFlux/shelter[screensTemperature] energyFlux/floor[temperature])"/>
			</box>
			<box name="temperatureCanopy" class="PlotR">
				<port name="ports" value="layers/*/temperature[value]"/> 
			</box>
			<box name="lightAbsorbed" class="PlotR">
				<port name="ports" value="layers/*/radiationAbsorbed[lightAbsorbed]"/> 
			</box>
			<box name="growthLightLwAbsorbed" class="PlotR">
				<port name="ports" value="layers/*/radiationAbsorbed[growthLightLwAbsorbed]"/> 
			</box>
			<box name="floorLwAbsorbed" class="PlotR">
				<port name="ports" value="layers/*/radiationAbsorbed[floorLwAbsorbed]"/> 
			</box>
			<box name="shelterLoss" class="PlotR">
				<port name="ports" value="layers/*/radiationAbsorbed[shelterLoss]"/> 
			</box>
			<box name="heatingAbsorbed" class="PlotR">
				<port name="ports" value="layers/*/radiationAbsorbed[heatingAbsorbed]"/> 
			</box>
		</box>
		<box name="controlled" class="PageR">
			<port name="ncol" value="3"/>
			<box name="airFlux" class="PlotR">
				<port name="ports" value="(given/airFlux[value] given/airFlux/infiltration[value] given/airFlux/crackVentilation[value] given/airFlux/gravitation[value])"/>
			</box>
			<box name="airCooling" class="PlotR">
				<port name="ports" value="(cooling/airSupplyMax[value] cooling/airSupplyMax/byWind[value] cooling/airSupplyMax/byTemp[value])"/>
			</box>
			<box name="coolingDemandSupply" class="PlotR">
				<port name="ports" value="(energyFlux/cooling/demand[value] energyFlux/cooling/supply[value] cooling/supply[slope] cooling/airFlux[value])"/>
			</box>
			<box name="setpoints" class="PlotR">
				<port name="ports" value="(setpoints/temperature/heating[value] setpoints/temperature/ventilation[value] setpoints/humidity/maximumRh[signal] heating/supply[value])"/>
			</box>
			<box name="status" class="PlotR">
				<port name="ports" value="(indoors/temperature[value] indoors/humidity[rh] indoors/co2[value] indoors/total/airFlux[value])"/>
			</box>
			<box name="pipes" class="PlotR">
				<port name="ports" value="pipes/*[temperature]"/>
			</box>
		</box>
		<box name="checkPipes" class="PageR">
			<port name="ncol" value="1"/>
			<box name="pipe1" class="PlotR">
				<port name="ports" value="(pipes/pipe1[nextTemperatureMin] pipes/pipe1[temperature] pipes/pipe1[nextTemperatureMax])"/>
			</box>
			<box name="pipe2" class="PlotR">
				<port name="ports" value="(pipes/pipe2[nextTemperatureMin] pipes/pipe2[temperature] pipes/pipe2[nextTemperatureMax])"/>
			</box>
		</box>
		<box name="production" class="PageR">
			<port name="ncol" value="2"/>
			<box name="heating" class="PlotR">
				<port name="ports" value="(budget[heatingEnergyFlux] budget[heatingEnergyTotal] indoors/temperature[value])"/>
			</box>
			<box name="growthLights" class="PlotR">
				<port name="ports" value="(budget[growthLightsEnergyFlux] growthLights[parEmission] budget[growthLightsEnergyTotal] )"/>
			</box>
			<box name="co2" class="PlotR">
				<port name="ports" value="(budget[co2Flux] budget[co2Total] indoors/co2[value])"/>
			</box>
			<box name="crop" class="PlotR">
				<port name="ports" value="(crop/growth/Pg[value] crop/mass[fruitGrowthRate] crop/yield[freshWeight])"/>
			</box>
		</box>
		<box name="photosynthesis" class="PageR">
			<port name="ncol" value="5"/>
			<box name="Pn" class="PlotR">
				<port name="ports" value="*/photosynthesis[Pn]"/>
			</box>
			<box name="Pg" class="PlotR">
				<port name="ports" value="*/photosynthesis[Pg]"/>
			</box>
			<box name="rbH2o" class="PlotR">
				<port name="ports" value="*/rb[rbH2O]"/>
			</box>
			<box name="rbCo2" class="PlotR">
				<port name="ports" value="*/rb[rbCo2]"/>
			</box>
			<box name="rsH2o" class="PlotR">
				<port name="ports" value="*/rs[rsH2O]"/>
			</box>
			<box name="rsCo2" class="PlotR">
				<port name="ports" value="*/rs[rsCo2]"/>
			</box>
			<box name="LUE" class="PlotR">
				<port name="ports" value="*/photosynthesis/lightResponse[LUE]"/>
			</box>
			<box name="Pnmax" class="PlotR">
				<port name="ports" value="*/photosynthesis/lightResponse[Pnmax]"/>
			</box>
			<box name="Pgmax" class="PlotR">
				<port name="ports" value="*/photosynthesis/lightResponse[Pgmax]"/>
			</box>
			<box name="climate" class="PlotR">
				<port name="ports" value="(indoors/temperature[value] indoors/humidity[rh] indoors/co2[value] indoors/total/airFlux[value])"/>
			</box>
		</box>
	</box>
	
</box> </xsl:template>

</xsl:stylesheet>
