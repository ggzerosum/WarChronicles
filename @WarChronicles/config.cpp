// We include basic defines, this is common shit used by everything. 
#include "basicdefines_A3.hpp"

// THis is where we "name" our mod. 
class CfgPatches
{
	class WC_WWII
	{
		units[] = {"F_Classname_U"};
		requiredAddons[] = {"Penis, Balls"}; 
		weapons[] = {};
		requiredVersion = 1.50;
	};
};

// The Below Configs will likely need to be separated out later, they are only here for template / demo sake: 
//
//
//

class CfgVehicleClasses
{
	class WC_US_Veh
	{
		displayName = "WWII US Vehicles";
		priority = -1;
	};	
	
	class WC_GR_Veh
	{
		displayName = "WWII GR Vehicles";
		priority = -1;
	};

	class WC_US_Mar
	{
		displayName = "WWII US Maritime";
		priority = -1;
	};
	
	class WC_GR_Mar
	{
		displayName = "WWII GR Maritime";
		priority = -1;
	};


	class WC_US_Air
	{
		displayName = "WWII US Aircraft";
		priority = -1;
	};

	class WC_GR_Air
	{
		displayName = "WWII GR Aircraft";
		priority = -1;
	};

};

class WeaponFireGun;
class WeaponCloudsGun;
class WeaponFireMGun;
class WeaponCloudsMGun;

class cfgVehicles
{
	class Car {};
	class Car_F : Car {};

	class WC_US_Car1 : Car_F {

		displayName = "Placeholder For US Vehicle";     // Displayname in editor
		model = "\WarChronicles\Models\Example.p3d"; // our p3d model path
		icon = "iconObject_1x2";
		picture = "iconObject_1x2";
		vehicleClass = "WC_US_Veh";
		scope = 2; 			/// makes the car visible in editor
		scopeCurator = 2;			// scope 2 means it's available in Zeus mode (0 means hidden)
		crew = "B_Faggot_F"; 	/// we need someone to fit into the car
		side = 3; 			/// civilian car should be on civilian side
		faction = BLU_F;
		terrainCoef = 1; 	/// different surface affects this car more, stick to tarmac
		turnCoef = 1; 	/// should match the wheel turn radius
		precision = 10; 	/// how much freedom has the AI for its internal waypoints - lower number means more precise but slower approach to way
		brakeDistance = 3.0; 	/// how many internal waypoints should the AI plan braking in advance
		acceleration = 15; 	/// how fast acceleration does the AI think the car has

		fireResistance = 15; 	/// lesser protection against fire than tanks
		armor = 32; 	/// just some protection against missiles, collisions and explosions
		cost = 50000; /// how likely is the enemy going to target this vehicle

		transportMaxBackpacks = 2; /// just some backpacks fit the trunk by default
		transportSoldier = 1; /// number of cargo except driver
		castDriverShadow = 1;
		castCargoShadow = 1;

		class TransportItems /// some first aid kits in trunk according to safety regulations
		{
			class _xx_FirstAidKit
			{
				name = "FirstAidKit";
				count = 15;
			};
			class _xx_Toolkit
			{
				name = "Toolkit";
				count = 1;
			};
			class _xx_Medikit
			{
				name = "Medikit";
				count = 2;
			};
		};

		driverAction = driver_boat01; /// what action is going the driver take inside the vehicle. Non-existent action makes the vehicle inaccessible
		cargoAction[] = {
			"passenger_injured_medevac_truck03",
			"passenger_injured_medevac_truck02",
			"passenger_injured_medevac_truck01",
			"passenger_low01"
		};
		getInAction = GetInLow; 		/// how does driver look while getting in
		getOutAction = GetOutMedium; 		/// and out
		cargoGetInAction[] = { "GetInLow" }; 	/// and the same for the rest, if the array has fewer members than the count of crew, the last one is used for the rest
		cargoGetOutAction[] = { "GetOutLow" }; 	/// that means all use the same in this case

#include "Scripts\physx.hpp"	/// PhysX settings are in a separate file to make this one simple



		// Must be kept as fail-safe in case of issue with the function 
		hiddenSelectionsTextures[] = { "\A3\Weapons_F\Data\placeholder_co.paa" };	 /// we could use any texture to cover the car

	};

}; 

/// All firemodes, to be sure
class Mode_SemiAuto;
class Mode_Burst;
class Mode_FullAuto;

/// Weapon slots
class SlotInfo;
class MuzzleSlot;
class CowsSlot;
class PointerSlot;
class UnderBarrelSlot;

#include "cfgRecoils.hpp" /// specific recoil patterns for this rifle
#include "cfgMagazines.hpp" /// specific magazines for this rifle
#include "cfgAmmo.hpp" /// specific ammo for this rifle

class CfgWeapons
{
	class Rifle;
	class Rifle_Base_F : Rifle
	{
		class WeaponSlotsInfo;
		class GunParticles;
	};
	class UGL_F;

	class Test_weapon_01_Base_F : Rifle_Base_F /// Just basic values common for all testing rifle variants
	{
		magazines[] = { 30Rnd_test_mag_Tracer, test_mag_group }; /// original custom made magazines and a group of several standardized mags
		reloadAction = "GestureReloadMX"; /// MX hand animation actually fits this rifle well
		magazineReloadSwitchPhase = 0.4; /// part of reload animation when new magazine ammo count should affect "revolving" animation source
		discreteDistanceInitIndex = 0; /// Ironsight zeroing is the lowest value by default

		// Size of recoil sway of the cursor
		maxRecoilSway = 0.0125;
		// Speed at which the recoil sway goes back to zero (from maxRecoilSway to 0 in 1/swayDecaySpeed seconds)
		swayDecaySpeed = 1.25;
		/// inertia coefficient of the weapon
		inertia = 0.5;

		/// positive value defines speed of the muzzle independent on the magazine setting, negative value is a coefficient of magazine initSpeed
		initSpeed = -1; /// this means that initSpeed of magazine is used

		class GunParticles : GunParticles
		{
			class SecondEffect
			{
				positionName = "Nabojnicestart";
				directionName = "Nabojniceend";
				effectName = "CaselessAmmoCloud";
			};
		};

		class WeaponSlotsInfo : WeaponSlotsInfo
		{
			class MuzzleSlot : MuzzleSlot
			{
				linkProxy = "\A3\data_f\proxies\weapon_slots\MUZZLE"; 	/// this can be set, but having some common helps a bit
				compatibleItems[] = { "test_suppressor" }; 				/// A custom made suppressor for this weapon
				iconPosition[] = { 0.0, 0.45 };							/// position of the slot icon inside of the weapon icon, relative to top-left corner in {right, down} format
				iconScale = 0.2;										/// scale of icon described in iconPicture
				iconPicture = "\A3\Weapons_F\Data\UI\attachment_muzzle.paa"; 	/// icon for selected slot
				iconPinpoint = "Center"; 										/// top, bottom, left, right, center alignment of the icon on snap point
			};
			class CowsSlot : CowsSlot /// default accessories for this slot
			{
				iconPosition[] = { 0.5, 0.35 };
				iconScale = 0.2;
			};
			class PointerSlot : PointerSlot /// default accessories for this slot
			{
				iconPosition[] = { 0.20, 0.45 };
				iconScale = 0.25;
			};
			class UnderBarrelSlot : UnderBarrelSlot /// using test bipod
			{
				iconPosition[] = { 0.2, 0.7 };
				iconScale = 0.2;
				compatibleItems[] = { "test_bipod_01_F" };
			};
		};

		/////////////////////////////////////////////////////  I R O N S I G H T S  /////////////////////////////////////////////////////
		opticsZoomMin = 0.375;
		opticsZoomMax = 1.1;
		opticsZoomInit = 0.75;

		distanceZoomMin = 300;
		distanceZoomMax = 300;
		/////////////////////////////////////////////////////  I R O N S I G H T S  /////////////////////////////////////////////////////

		descriptionShort = "Testing weapon with grenade launcher"; /// displayed on mouseOver in Inventory
		handAnim[] = { "OFP2_ManSkeleton", "\A3\Weapons_F\Rifles\MX\data\Anim\MX_gl.rtm" }; /// MX hand animation actually fits this rifle well
		dexterity = 1.8;

		//caseless ammo//
		caseless[] = { "", 1, 1, 1 };  /// no sound of ejected brass
		soundBullet[] = { caseless, 1 };

		selectionFireAnim = "muzzleFlash"; /// are we able to get rid of all the zaslehs?

		modes[] = { Single, FullAuto, fullauto_medium, single_medium_optics1, single_far_optics2 }; /// Includes fire modes for AI

		////////////////////////////////////////////////////// NO OPTICS ///////////////////////////////////////////////////////////		

		class Single : Mode_SemiAuto /// Pew
		{
			sounds[] = {		/// the new parameter to distinguish muzzle accessories type
				StandardSound, // default zvuk
				SilencedSound // silenced zvuk
			};

			class BaseSoundModeType
			{

				weaponSoundEffect = "DefaultRifle"; /// custom made sounds

				closure1[] = { "A3\sounds_f\weapons\closure\closure_rifle_6", db3, 1, 10 }; /// custom made sounds
				closure2[] = { "A3\sounds_f\weapons\closure\closure_rifle_7", db3, 1, 10 }; /// custom made sounds
				soundClosure[] = { closure1, 0.5, closure2, 0.5 }; /// custom made sounds
			};

			class StandardSound : BaseSoundModeType
			{
				begin1[] = { "A3\sounds_f\weapons\MX\mx-st-full-1", db3, 1, 1200 }; /// custom made sounds
				begin2[] = { "A3\sounds_f\weapons\MX\mx-st-full-2", db3, 1, 1200 }; /// custom made sounds
				soundBegin[] = { begin1, 0.5, begin2, 0.5 }; /// custom made sounds
			};

			class SilencedSound : BaseSoundModeType
			{
				begin1[] = { "A3\sounds_f\weapons\silenced\silent-18", db - 2, 1, 100 };
				begin2[] = { "A3\sounds_f\weapons\silenced\silent-19", db - 2, 1, 100 };
				begin3[] = { "A3\sounds_f\weapons\silenced\silent-11", db - 2, 1, 100 };
				soundBegin[] = { begin1, 0.333, begin2, 0.333, begin3, 0.333 };
			};

			reloadTime = 0.096; /// means some 625 rounds per minute
			dispersion = 0.00087; /// A bit less than 3 MOA

			recoil = "recoil_single_Test_rifle_01"; /// defined in cfgRecoils
			recoilProne = "recoil_single_prone_Test_rifle_01"; /// defined in cfgRecoils

			minRange = 2; minRangeProbab = 0.5; 	/// Task Force Balance black magic - this is the probability which AI thinks it would hit target at set range with
			midRange = 200; midRangeProbab = 0.7; 	/// it is no real probability of hit, just used for AI to compute if the shot is worth to take - AI chooses highest
			maxRange = 400; maxRangeProbab = 0.3; 	/// probability of the weapon, does some calculation and compares it with calculated probability of other weapons
		};

		class FullAuto : Mode_FullAuto /// Pew-pew-pew-pew-pew
		{
			sounds[] = {		/// the new parameter to distinguish muzzle accessories type
				StandardSound, // default zvuk
				SilencedSound // silenced zvuk
			};

			class BaseSoundModeType
			{

				weaponSoundEffect = "DefaultRifle"; /// custom made sounds

				closure1[] = { "A3\sounds_f\weapons\closure\closure_rifle_6", db3, 1, 10 }; /// custom made sounds
				closure2[] = { "A3\sounds_f\weapons\closure\closure_rifle_7", db3, 1, 10 }; /// custom made sounds
				soundClosure[] = { closure1, 0.5, closure2, 0.5 }; /// custom made sounds
			};

			class StandardSound : BaseSoundModeType
			{
				begin1[] = { "A3\sounds_f\weapons\MX\mx-st-full-1", db3, 1, 1200 }; /// custom made sounds
				begin2[] = { "A3\sounds_f\weapons\MX\mx-st-full-2", db3, 1, 1200 }; /// custom made sounds
				soundBegin[] = { begin1, 0.5, begin2, 0.5 }; /// custom made sounds
			};

			class SilencedSound : BaseSoundModeType
			{
				begin1[] = { "A3\sounds_f\weapons\silenced\silent-18", db - 2, 1, 100 };
				begin2[] = { "A3\sounds_f\weapons\silenced\silent-19", db - 2, 1, 100 };
				begin3[] = { "A3\sounds_f\weapons\silenced\silent-11", db - 2, 1, 100 };
				soundBegin[] = { begin1, 0.333, begin2, 0.333, begin3, 0.333 };
			};

			reloadTime = 0.096;
			dispersion = 0.00087;

			recoil = "recoil_auto_Test_rifle_01"; /// defined in cfgRecoils
			recoilProne = "recoil_auto_prone_Test_rifle_01"; /// defined in cfgRecoils

			minRange = 0; minRangeProbab = 0.9;
			midRange = 15; midRangeProbab = 0.7;
			maxRange = 30; maxRangeProbab = 0.1;

			aiRateOfFire = 0.000001;
		};

		class fullauto_medium : FullAuto /// Pew, pew, pew only for AI
		{
			showToPlayer = 0;
			burst = 3;

			minRange = 2; minRangeProbab = 0.5;
			midRange = 75; midRangeProbab = 0.7;
			maxRange = 150; maxRangeProbab = 0.05;

			aiRateOfFire = 2.0;
			aiRateOfFireDistance = 200;
		};
		//////////////////////////////////////////////////// OPTICS //////////////////////////////////////////////////

		class single_medium_optics1 : Single /// Pew for AI with collimator sights
		{
			requiredOpticType = 1;
			showToPlayer = 0;

			minRange = 2; minRangeProbab = 0.2;
			midRange = 450; midRangeProbab = 0.7;
			maxRange = 600; maxRangeProbab = 0.2;

			aiRateOfFire = 6;
			aiRateOfFireDistance = 600;
		};

		class single_far_optics2 : single_medium_optics1	/// Pew for AI with better sights
		{
			requiredOpticType = 2;
			showToPlayer = 0;

			minRange = 100; minRangeProbab = 0.1;
			midRange = 500; midRangeProbab = 0.6;
			maxRange = 700; maxRangeProbab = 0.05;

			aiRateOfFire = 8;
			aiRateOfFireDistance = 700;
		};

		class Test_GL_F : UGL_F /// Some grenade launcher to have some more fun
		{
			displayName = "Test grenade launcher";
			descriptionShort = "TGL";
			useModelOptics = false;
			useExternalOptic = false; /// Doesn't use optics from the attachment, has it's own
			magazines[] = { 1Rnd_HE_Grenade_shell };
			cameraDir = "OP_look";
			discreteDistance[] = { 100, 200, 300, 400 };
			discreteDistanceCameraPoint[] = { "OP_eye", "OP_eye2", "OP_eye3", "OP_eye4" }; /// the angle of gun changes with zeroing
			discreteDistanceInitIndex = 1; /// 200 is the default zero
		};

		aiDispersionCoefY = 6.0; /// AI should have some degree of greater dispersion for initial shoots
		aiDispersionCoefX = 4.0; /// AI should have some degree of greater dispersion for initial shoots
		drySound[] = { "A3\sounds_f\weapons\Other\dry_1", db - 5, 1, 10 }; /// custom made sounds
		reloadMagazineSound[] = { "A3\sounds_f\weapons\reloads\new_MX", db - 8, 1, 30 }; /// custom made sounds
	};

	class Test_weapon_01_F : Test_weapon_01_Base_F
	{
		scope = 2; /// should be visible and useable in game
		displayName = "Test weapon"; /// some name
		model = "\Samples_f\Test_weapon_01\test_weapon_01_F.p3d"; /// path to model

		picture = "\Samples_f\Test_weapon_01\Data\UI\gear_test_weapon_01_X_CA.paa"; /// different accessories have M, S, T instead of X
		UiPicture = "\A3\Weapons_F\Data\UI\icon_gl_CA.paa"; /// weapon with grenade launcher should be marked such way

		weaponInfoType = "RscWeaponZeroing"; /// display with zeroing is good for iron sights

		muzzles[] = { this, Test_GL_F }; /// to be able to switch between bullet muzzle and TGL

		class WeaponSlotsInfo : WeaponSlotsInfo
		{
			mass = 100; /// some rough estimate
		};
	};

	/**** SLOTABLE WEAPONS ****/

	class Test_weapon_01_holo_pointer_F : Test_weapon_01_F /// standard issue variant with holo optics and laser pointer
	{
		class LinkedItems
		{
			class LinkedItemsOptic
			{
				slot = "CowsSlot";
				item = "optic_Holosight";
			};
			class LinkedItemsAcc
			{
				slot = "PointerSlot";
				item = "acc_pointer_IR";
			};
		};
	};

	/// include accessory from separate file to not clutter this one
#include "accessory.hpp"
};
