				topLeft					= "HUD_top_left";
				topRight				= "HUD_top_right";
				bottomLeft				= "HUD_bottom_left";
				/// adjust position inside of memory points LOD
				borderLeft				= 0;
				borderRight				= 0;
				borderTop				= 0;
				borderBottom			= 0.0;
				color[]					= {0.15, 0.15, 1.0, 1.0};
				enableParallax			= false;

				class Bones
				{
				};

				class Draw
				{
					color[]				= {0.07, 0.67, 0.745};
					alpha				= 1;

					class ClockHour
					{
						type			= text;
						source			= ClockHour;
						sourceScale		= 12;	/// the original source is on the scale (0-1), it needs to be multiplied

						align			= right;
						scale			= 2;
						pos[]			= {{0.05, 0.00}, 1};
						right[]			= {{0.40, 0.00}, 1};
						down[]			= {{0.05, 1.00}, 1};
					};

					class ClockMinute
					{
						type			= text;
						source			= ClockMinute;
						sourceScale		= 60;	/// the original source is on the scale (0-1), it needs to be multiplied

						align			= right;
						scale			= 2;
						pos[]			= {{0.55, 0.00}, 1};
						right[]			= {{0.90, 0.00}, 1};
						down[]			= {{0.55, 1.00}, 1};
					};
				};