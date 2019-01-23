/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software  Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2005 by the Blender Foundation.
 * All rights reserved.
 * ***** END GPL LICENSE BLOCK *****
 *
 */

/** \file blender/modifiers/intern/MOD_functiondeform.c
 *  \ingroup modifiers
 *
 */

#include "MEM_guardedalloc.h"

#include "DNA_mesh_types.h"
#include "DNA_modifier_types.h"
#include "DNA_meshdata_types.h"
#include "DNA_object_types.h"

#include "BKE_mesh.h"
#include "BKE_modifier.h"
#include "BKE_scene.h"

#include "BLI_math.h"
#include "BLI_utildefines.h"

#include "MOD_util.h"

#include "DEG_depsgraph.h"
#include "DEG_depsgraph_query.h"
#include "time.h"

#include "FN_functions.h"

static void deformVerts(
        ModifierData *md,
        const ModifierEvalContext *ctx,
        Mesh *UNUSED(mesh),
        float (*vertexCos)[3],
        int numVerts)
{
	FunctionRef fn = FN_get_deform_function();
	FnInputsRef fn_in = FN_inputs_new(fn);
	FnOutputsRef fn_out = FN_outputs_new(fn);

	float input[3] = {1, 2, 3};
	float control = 10;
	FN_inputs_set_index(fn_in, 0, input);
	FN_inputs_set_index(fn_in, 1, &control);

	FN_function_call(fn, fn_in, fn_out);

	float result[3];
	FN_outputs_get_index(fn_out, 0, result);

	printf("Result: %f %f %f\n", result[0], result[1], result[2]);

	printf("Finished\n");
}


static void initData(ModifierData *md)
{
	FunctionDeformModifierData *fdmd = (FunctionDeformModifierData *)md;
	fdmd->control1 = 0.0f;
	fdmd->control2 = 0;
}

static bool dependsOnTime(ModifierData *UNUSED(md))
{
	return true;
}


ModifierTypeInfo modifierType_FunctionDeform = {
	/* name */              "Function Deform",
	/* structName */        "FunctionDeformModifierData",
	/* structSize */        sizeof(FunctionDeformModifierData),
	/* type */              eModifierTypeType_OnlyDeform,
	/* flags */             eModifierTypeFlag_AcceptsMesh,
	/* copyData */          modifier_copyData_generic,

	/* deformVerts_DM */    NULL,
	/* deformMatrices_DM */ NULL,
	/* deformVertsEM_DM */  NULL,
	/* deformMatricesEM_DM*/NULL,
	/* applyModifier_DM */  NULL,

	/* deformVerts */       deformVerts,
	/* deformMatrices */    NULL,
	/* deformVertsEM */     NULL,
	/* deformMatricesEM */  NULL,
	/* applyModifier */     NULL,

	/* initData */          initData,
	/* requiredDataMask */  NULL,
	/* freeData */          NULL,
	/* isDisabled */        NULL,
	/* updateDepsgraph */   NULL,
	/* dependsOnTime */     dependsOnTime,
	/* dependsOnNormals */	NULL,
	/* foreachObjectLink */ NULL,
	/* foreachIDLink */     NULL,
	/* foreachTexLink */    NULL,
};