macroScript P3D_Export category: "VM`s" ToolTip:"P3D_Export"
(
-- vars start
	export_path = "C:\\temp.txt"
	Local TotalProgress = 0
	Local O2Path = "C:\\bis\\Oxygen\\O2Script.exe "
	Local O2Script = "convertBiTXT.bio2s "
	local bone_names = #();
	local vertex_weight = #();
	local selections_vertex_weight = #();
	local selections_bone_names = #();

	local exportFn = ()
-- vars end

-- UI start
	rollout p3d_export "P3D Export" width:200 height:250
	(
		label exp "File:" width:150
		
		button saveas "Save As" width:150 height:25 tooltip:""
		on saveas pressed do
		(
			export_path = getSaveFileName()
			if export_path==undefined then
			(
				export_path = "c:\\temp.txt" 
			)
			if export_path!=undefined then
			(
				export_path_filename = filenameFromPath export_path as string
				if (export_path_filename.count)>25 then
				(
				export_path_filename = filenameFromPath export_path as string
				export_path_filenamePath = getFilenamePath export_path as string
				export_path_filename = ((substring export_path_filename 1 25) as string + ".txt")
				export_path = (export_path_filenamePath + export_path_filename)
				)
				if (export_path!=undeffined) then
				(
					p3d_export.exp.text = ("Output : "+filenameFromPath export_path as string)
					p3d_export.saveas.tooltip = export_path
				)
			)
		)
		
		button export "Export" width:150 height:25
		on export pressed do
		(
			exportFn()
		)
		checkbox GenerateMatName "Generate Material Name" pos:[10,90]
		checkbox SkinExport "Export Skin (if exists)" checked:true pos:[10,110]
		checkbox SelectionExport "Export NamedSelections(SKIN)" checked:true pos:[10,130]
		checkbox SelectionExportNormalize "Normalize" checked:true pos:[35,150]
		checkbox MatColor "Assign Material Color" checked:false pos:[10,170]
		
		--checkbox LayerName "LOD from Layer Name" checked:true

		
		progressbar export_progress color:red pos:[10,190]
	)
-- UI end
-- funkce start
	-- Export funkce start
	function exportFn =
	(
		with redraw off
		(
			undo off
			max create mode
		
			-- otevoení / vytvooení souboru start
			out_file = openFile export_path mode:"w"
			if out_file == undefined then
			(
				out_file = createFile export_path
			)
			-- otevoení / vytvooení souboru end
			-- hlavieka start
			format ":header\n" to:out_file
			format "version 1.0\n" to:out_file
			format "sharp SG\n" to:out_file
			-- hlavieka end
		
			-- eíslo LODu start
			lod_name = "1.000"
			format (":lod \""+ lod_name +"\"\n") to:out_file
			-- eíslo LODu end
			
			seltmp = getCurrentSelection()

			-- výpis bodu a facu start
			for i in seltmp do 
			(
				-- reset Vars start
				bone_names = #();
				vertex_weight = #();
				selections_vertex_weight = #();
				selections_bone_names = #();
				-- reset Vars end
				
				-- kopie objektu do temp_obj start
				temp_obj = copy i
				temp_obj.wirecolor = i.wirecolor
				temp_obj.name = i.name
					selection = temp_obj
				-- kopie objektu do temp_obj end
				
				-- Skin start
				if p3d_export.SkinExport.checked then
				(
					if temp_obj.modifiers[#Skin]!=undefined then
					(
						-- Skin editable poly reset start
						max modify mode
						modPanel.setCurrentObject temp_obj.baseObject;
						max create mode
						max modify mode
						modPanel.setCurrentObject temp_obj.modifiers[#Skin];
						-- Skin editable poly reset start
					
						SkinModifier = temp_obj.skin
						-- jednotlivy vertex start
						-- seznam ID kostí start
						for bone_num = 1 to (skinOps.GetNumberBones SkinModifier) do
						(
							append vertex_weight #();
							vertex_weight[bone_num][(skinOps.GetNumberVertices SkinModifier)] = undefined
							append bone_names (skinOps.GetBoneName SkinModifier bone_num 1)
						)
						-- seznam ID kostí end
							
						for vertex_num = 1 to (skinOps.GetNumberVertices SkinModifier) do
						(
							-- jednotliva kost pro jednotlivy vertex start
							for bone_num = 1 to (skinOps.GetVertexWeightCount SkinModifier vertex_num) do
							(
								cislo_kosti = skinOps.GetVertexWeightBoneID SkinModifier vertex_num bone_num -- Bone ID
								
								vertex_weight[cislo_kosti][vertex_num] = (skinOps.GetVertexWeight SkinModifier vertex_num bone_num) -- pridavani váhy vertexu do seznamu vertexu
							)
							-- jednotliva kost pro jednotlivy vertex end
						)
						-- jednotlivy vertex start
					)
					max create mode
					--deleteModifier $ 1;
				)
				-- Skin end
				max create mode
				max modify mode
				-- Selections start (Uses SKIN)
				if p3d_export.SelectionExport.checked then
				(
					if temp_obj.modifiers[#Selections]!=undefined then
					(
						max modify mode
						-- Skin editable poly reset start
						modPanel.setCurrentObject $.baseObject
						max create mode
						max modify mode
						modPanel.setCurrentObject $.modifiers[#Selections]
						-- Skin editable poly reset start
					
						Selection_SkinModifier = $.modifiers[#Selections]
					
						-- jednotlivy vertex start
						-- seznam ID kostí start
						for bone_num = 1 to (skinOps.GetNumberBones Selection_SkinModifier) do
						(
							append selections_vertex_weight #();
							selections_vertex_weight[bone_num][(skinOps.GetNumberVertices Selection_SkinModifier)] = undefined
							append selections_bone_names (skinOps.GetBoneName Selection_SkinModifier bone_num 1)
						)
							
						for Selection_vertex_num = 1 to (skinOps.GetNumberVertices Selection_SkinModifier) do
						(
							-- jednotliva kost pro jednotlivy vertex start
							for selections_bone_num = 1 to (skinOps.GetVertexWeightCount Selection_SkinModifier Selection_vertex_num) do
							(
								cislo_kosti = skinOps.GetVertexWeightBoneID Selection_SkinModifier Selection_vertex_num selections_bone_num -- Bone ID
								selections_vertex_weight[cislo_kosti][Selection_vertex_num] = (skinOps.GetVertexWeight Selection_SkinModifier Selection_vertex_num selections_bone_num) -- pridavani váhy vertexu do seznamu vertexu
							)
						-- jednotliva kost pro jednotlivy vertex end
						)
					
						-- seznam ID kostí end
							
						-- jednotlivy vertex END
					)
					max create mode
					-- Normalizace selekcí START
					if p3d_export.SelectionExportNormalize.checked then
					(
						for n=1 to selections_vertex_weight.count do
						(
							for i=1 to selections_vertex_weight[n].count do
							(
								if (selections_vertex_weight[n][i]!=undefined) then
								(
									for b=(n+1) to selections_vertex_weight.count do
									(
										if ((selections_vertex_weight[b][i]!=undefined) AND (selections_vertex_weight[n][i]!=undefined)) then
										(
											if (selections_vertex_weight[n][i]>=selections_vertex_weight[b][i]) then
											(
												selections_vertex_weight[b][i]=undefined
												selections_vertex_weight[n][i]=1.0
											)
										)
										
										if ((selections_vertex_weight[b][i]!=undefined) AND (selections_vertex_weight[n][i]!=undefined)) then
										(
											if (selections_vertex_weight[n][i]<selections_vertex_weight[b][i]) then
											(
												selections_vertex_weight[b][i]=1.0
												selections_vertex_weight[n][i]=undefined
											)
										)
									)
								)
							)
						)
					)
					-- Normalizace selekcí END
					--deleteModifier $ 1;
				)
				-- Selections end (Uses SKIN)
			
				-- konverze do EditablePoly start	
				if (classOf temp_obj)!=Editable_Poly then
				(
					convertTo temp_obj PolyMeshObject
				)
				-- limit polygon sides start
				max modify mode
				addModifier temp_obj (Turn_to_Poly ())
				temp_obj.modifiers[#Turn_to_Poly].limitPolySize = on
				temp_obj.modifiers[#Turn_to_Poly].maxPolySize = 4
				temp_obj.modifiers[#Turn_to_Poly].keepConvex = on
				temp_obj.modifiers[#Turn_to_Poly].removeMidEdgeVertices = off
				collapseStack temp_obj
				max create mode
				-- limit polygon sides end
			-- konverze do EditablePoly end
		
			-- jednotlivý objekt start
				-- eíslo LODu start
				/*lod_name = "1.000"
					-- LayerName start
					if p3d_export.LayerName.checked then
					(
					lod_name = i.layer.name
					)
					-- LayerName end
				format (":lod \""+ lod_name +"\"\n") to:out_file*/
				-- eíslo LODu end
				format (":object \""+ temp_obj.name +"\"\n") to:out_file
		
				-- pocet UV kanalu start
				format ("uvchannels ") to:out_file
				for UVchannel = 0 to ((polyOp.getNumMaps temp_obj)-2) do
				(
					format (UVchannel as string+" ") to:out_file
				)
				format ("\n") to:out_file
				-- pocet UV kanalu end
			
				format ":points\n" to:out_file
		
				-- jednotlivé vertexy objektu start
				for num = 1 to temp_obj.GetNumVertices() do
				(
					pozice_bodu = ((polyop.getVert temp_obj num))
					pozice_bodu = (pozice_bodu[1] as string + " " + pozice_bodu[2] as string + " " + pozice_bodu[3] as string)
					format (pozice_bodu +"\n") to:out_file				
				)
				format "\n" to:out_file
				-- jednotlivé vertexy objektu end
			
				-- jednotlivé facy objektu start
				for num = 1 to temp_obj.GetNumFaces() do
				(
			
					-- progress per object start
					p3d_export.export_progress.value = ((num as float)/(temp_obj.GetNumFaces() as float /100.0))
					-- progress per object end
					
					format ":face\n" to:out_file
					format "index " to:out_file
					-- vertexy facu start
					for vert = 1 to (temp_obj.GetFaceDegree num) do
					(
						format (temp_obj.GetFaceVertex num vert as string +" ") to:out_file
					)
					format "\n" to:out_file
				-- vertexy facu end
					-- UV start
					for UVchannel = 0 to ((polyOp.getNumMaps temp_obj)-2) do
					(
						format ("uv "+ UVchannel as string +" ") to:out_file
		
					-- každý vertex facu start
						for vert in (polyOp.getMapFace temp_obj (UVchannel+1) num) do
						(

							uvw = polyOp.getMapVert temp_obj (UVchannel+1) vert
							if uvw[1] as string =="-1.#IND" then uvw[1]=0
							if uvw[2] as string =="-1.#IND" then uvw[2]=0
							format (uvw[1] as string + " "+ uvw[2] as string +" ") to:out_file
						)
					-- každý vertex facu end
						format "\n" to:out_file
					)
					-- UV end
				
				-- smoothgroupy start
					smoothgroup = temp_obj.GetFaceSmoothingGroups num
					format ("sg "+ smoothgroup as string +"\n") to:out_file
				-- smoothgroupy end
				
				-- textury/materialy start
					-- Multimaterial start
					if temp_obj.material!=undefined then
					(
						if (getNumSubMtls temp_obj.material)>0 then 
						(
							if (temp_obj.material.materialList[(temp_obj.GetFaceMaterial num)])!=undefined then
							(
								textura = (temp_obj.material.materialList[(temp_obj.GetFaceMaterial num)].diffuseMap.filename)
								textura = substring textura 4 textura.count
							-- nenastavený materiál start
								mat = ""
								if p3d_export.GenerateMatName.checked then
								(
									mat = (substring textura 1 (textura.count-7))+".rvmat"
								)
								if (getFilenameType (temp_obj.material.materialList[(temp_obj.GetFaceMaterial num)].diffuseMap.name))=="" then
								(
									if p3d_export.GenerateMatName.checked then
									(
										mat = (substring textura 1 (textura.count-7))+".rvmat"
									)
									else
									(
										mat = ""
									)
								)
								else
								(
									mat = (temp_obj.material.materialList[(temp_obj.GetFaceMaterial num)].diffuseMap.name)
									mat = (substring mat 4 mat.count)

								)
							-- nenastavený materiál start
						
								format (("texture \""+ textura +"\"\n")) to:out_file
								format (("material \""+ mat +"\"\n")) to:out_file

							)
						)
					)
					-- Multimaterial end
					
					-- objekt bez materialu start
					if p3d_export.MatColor.checked then
					(
						if temp_obj.material==undefined then
						(
							r = (((temp_obj.wirecolor.r/(255/100))/100) as float) as string
							g = (((temp_obj.wirecolor.g/(255/100))/100) as float) as string
							b = (((temp_obj.wirecolor.b/(255/100))/100) as float) as string
							textura = "#(argb,8,8,3)color("+ r +","+ g + ","+ b + ",1.0,CO)"
							format (("texture \""+ textura +"\"\n")) to:out_file
						)
					)
					-- objekt bez materialu end
				
				-- textury/materialy end
				)
				-- jednotlivé facy objektu end
				
				
				-- výpis Skin selekcí start
				if p3d_export.SkinExport.checked then
				(
					for i = 1 to vertex_weight.count  do
					(
						format ":selection " to:out_file
						format ("\""+bone_names[i]+"\"\n") to:out_file
					
						for vertex=1 to (vertex_weight[i].count) do
						(
							if vertex_weight[i][vertex] != undefined then 
							(
								format (vertex as string) to:out_file
								format (" "+ (vertex_weight[i][vertex] as string)+" \n") to:out_file
							)					
						)
					)
				)
				-- výpis Skin selekcí end
				
				-- výpie Selections(SKIN) selekcí START
				if p3d_export.SelectionExport .checked then
				(
					for i = 1 to selections_vertex_weight.count  do
					(
						format ":selection " to:out_file
						format ("\""+selections_bone_names[i]+"\"\n") to:out_file
					
						for vertex=1 to (selections_vertex_weight[i].count) do
						(
							if selections_vertex_weight[i][vertex] != undefined then 
							(
							format (vertex as string) to:out_file
							format (" "+ ( selections_vertex_weight[i][vertex] as string)+" \n") to:out_file
							)
						
						)
					)
				)
				-- výpie Selections(SKIN) selekcí END
						
				delete temp_obj
				-- jednotlivý objekt end
			)
		-- výpis bodu a facu end


		-- konec souboru start
			format "\n:end" to:out_file
			close out_file
		-- konec souboru end
		
		-- progress reset start
			p3d_export.export_progress.value = 0;
		-- progress reset end
		)
		-- koverze do P3D start
			ShellLaunch O2Path (O2Script + export_path)
		-- koverze do P3D start
		
		)
	-- Export funkce end
-- funkce end

-- prvotní nastavení UI start
	createDialog p3d_export
	p3d_export.exp.text = ("Output : "+filenameFromPath export_path as string)
	p3d_export.saveas.tooltip = export_path
-- prvotní nastavení UI end
		
		
-- vars start
	--export_path = "C:\\temp.txt"
	--TotalProgress = 0
	--O2Path = "C:\\bis\\Oxygen\\O2Script.exe "
	--O2Script = "convertBiTXT.bio2s "
	--bone_names = #();
	--vertex_weight = #();
	--selections_vertex_weight = #();
	--selections_bone_names = #();

	--exportFn()
	
-- vars end
)		