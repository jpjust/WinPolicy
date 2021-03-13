/* Rescue disk management class
 *
 * Copyright (c) 2006 João Paulo Just Peixoto <just1982@gmail.com>.
 *
 * This file is part of WinPolicy.
 *
 * WinPolicy is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WinPolicy is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with WinPolicy.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _RDISKMGR_H
#define _RDISKMGR_H

#define RD_RSA_KEY_SIZE 1024
#define RD_RSA_KEY_N "y6w31ehx1025ggseb9wxub76gxo6p4xqsf739w6h0vbz8cvkjo3d1t4ss4kpatat8t0nd32u69h5duqdrlxgvfbbuoo9r8pu08psile65hexwtqtol8e1oy3ztsz0koxajakq2zui2o4dre5lsh26b6ycseywnr15owilck4g37rxptbp6cd76dy2pv9g74mryqcuh"
#define RD_RSA_KEY_E "y6w31ehx1025ggseb9wxub76gxo6p4xqsf739w6h0vbz8cvkjo3d1t4ss4kpatat8t0nd32u69h5duqdrlxgvfbbuoo9r8pu08ntneckpd95ylnplg828q85qwe87hly6rk7kgqtklgadany05jbrmizu20jprkhc57txy4o8ykzbmo6p0jydp3095zgzzdozayp3h"
/*#define RD_RSA_KEY_D "4vuapwxf055g7htrrbp9z6qqmzo0yg4tjhll1evs59ma6c4id3fwpz0ool83m46oqz03c5kz61cqheoc934s4hm6u3iw918u018jnwwnj1w5zst3nmw0bj6b3ucbghxzqoibd7j9d87gwr8kab2re8czz5fi8tnhwlbeka0o1a2zwio0yfia1yfuw5zxaa73kgztat"*/

class RDiskMgr
{
public:
	static bool SetRescuePwd(void);
	static bool CheckRescuePwd(bool Verbose);

private:
};

#endif
