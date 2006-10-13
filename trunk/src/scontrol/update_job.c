/*****************************************************************************\
 *  update_job.c - update job function for scontrol.
 *****************************************************************************
 *  Copyright (C) 2002-2006 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Morris Jette <jette1@llnl.gov>
 *  UCRL-CODE-217948.
 *  
 *  This file is part of SLURM, a resource management program.
 *  For details, see <http://www.llnl.gov/linux/slurm/>.
 *  
 *  SLURM is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *
 *  In addition, as a special exception, the copyright holders give permission 
 *  to link the code of portions of this program with the OpenSSL library under
 *  certain conditions as described in each individual source file, and 
 *  distribute linked combinations including the two. You must obey the GNU 
 *  General Public License in all respects for all of the code used other than 
 *  OpenSSL. If you modify file(s) with this exception, you may extend this 
 *  exception to your version of the file(s), but you are not obligated to do 
 *  so. If you do not wish to do so, delete this exception statement from your
 *  version.  If you delete this exception statement from all source files in 
 *  the program, then also delete it here.
 *  
 *  SLURM is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *  
 *  You should have received a copy of the GNU General Public License along
 *  with SLURM; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA.
\*****************************************************************************/

#include "scontrol.h"


/* 
 * scontrol_update_job - update the slurm job configuration per the supplied arguments 
 * IN argc - count of arguments
 * IN argv - list of arguments
 * RET 0 if no slurm error, errno otherwise. parsing error prints 
 *			error message and returns 0
 */
extern int
scontrol_update_job (int argc, char *argv[]) 
{
	int i, update_cnt = 0;
	job_desc_msg_t job_msg;

	slurm_init_job_desc_msg (&job_msg);	

	for (i=0; i<argc; i++) {
		if (strncasecmp(argv[i], "JobId=", 6) == 0)
			job_msg.job_id = 
				(uint32_t) strtol(&argv[i][6], 
						 (char **) NULL, 10);
		else if (strncasecmp(argv[i], "TimeLimit=", 10) == 0) {
			if ((strcasecmp(&argv[i][10], "UNLIMITED") == 0) ||
			    (strcasecmp(&argv[i][10], "INFINITE") == 0))
				job_msg.time_limit = INFINITE;
			else
				job_msg.time_limit = 
					(uint32_t) strtol(&argv[i][10], 
							  (char **) NULL, 10);
			update_cnt++;
		}
		else if (strncasecmp(argv[i], "Priority=", 9) == 0) {
			job_msg.priority = 
				(uint32_t) strtoll(&argv[i][9], 
						(char **) NULL, 10);
			update_cnt++;
		}
		else if (strncasecmp(argv[i], "Nice=", 5) == 0) {
			int nice;
			nice = strtoll(&argv[i][5], (char **) NULL, 10);
			if (abs(nice) > NICE_OFFSET) {
				error("Invalid nice value, must be between "
					"-%d and %d", NICE_OFFSET, NICE_OFFSET);
				exit_code = 1;
				return 0;
			}
			job_msg.nice = NICE_OFFSET + nice;
			update_cnt++;
		}
		else if (strncasecmp(argv[i], "Nice", 4) == 0) {
			job_msg.nice = NICE_OFFSET + 100;
			update_cnt++;
		}		
		else if (strncasecmp(argv[i], "ReqProcs=", 9) == 0) {
			job_msg.num_procs = 
				(uint32_t) strtol(&argv[i][9], 
						(char **) NULL, 10);
			update_cnt++;
		}
		else if ((strncasecmp(argv[i], "MinNodes=", 9) == 0) ||
		         (strncasecmp(argv[i], "ReqNodes=", 9) == 0)) {
			job_msg.min_nodes = 
				(uint32_t) strtol(&argv[i][9],
						 (char **) NULL, 10);
			update_cnt++;
		}
		else if (strncasecmp(argv[i], "ReqSockets=", 11) == 0) {
			job_msg.min_sockets = 
				(uint32_t) strtol(&argv[i][11],
						 (char **) NULL, 10);
			update_cnt++;
		}
		else if (strncasecmp(argv[i], "ReqCores=", 9) == 0) {
			job_msg.min_cores = 
				(uint32_t) strtol(&argv[i][9],
						 (char **) NULL, 10);
			update_cnt++;
		}
		else if (strncasecmp(argv[i], "ReqThreads=", 11) == 0) {
			job_msg.min_threads = 
				(uint32_t) strtol(&argv[i][11],
						 (char **) NULL, 10);
			update_cnt++;
		}
		else if (strncasecmp(argv[i], "MinProcs=", 9) == 0) {
			job_msg.job_min_procs = 
				(uint32_t) strtol(&argv[i][9], 
						(char **) NULL, 10);
			update_cnt++;
		}
		else if (strncasecmp(argv[i], "MinSockets=", 11) == 0) {
			job_msg.job_min_sockets = 
				(uint32_t) strtol(&argv[i][11], 
						(char **) NULL, 10);
			update_cnt++;
		}
		else if (strncasecmp(argv[i], "MinCores=", 9) == 0) {
			job_msg.job_min_cores = 
				(uint32_t) strtol(&argv[i][9], 
						(char **) NULL, 10);
			update_cnt++;
		}
		else if (strncasecmp(argv[i], "MinThreads=", 11) == 0) {
			job_msg.job_min_threads = 
				(uint32_t) strtol(&argv[i][11], 
						(char **) NULL, 10);
			update_cnt++;
		}
		else if (strncasecmp(argv[i], "MinMemory=", 10) == 0) {
			job_msg.job_min_memory = 
				(uint32_t) strtol(&argv[i][10], 
						(char **) NULL, 10);
			update_cnt++;
		}
		else if (strncasecmp(argv[i], "MinTmpDisk=", 11) == 0) {
			job_msg.job_min_tmp_disk = 
				(uint32_t) strtol(&argv[i][11], 
						(char **) NULL, 10);
			update_cnt++;
		}
		else if (strncasecmp(argv[i], "Partition=", 10) == 0) {
			job_msg.partition = &argv[i][10];
			update_cnt++;
		}
		else if (strncasecmp(argv[i], "Name=", 5) == 0) {
			job_msg.name = &argv[i][5];
			update_cnt++;
		}
		else if (strncasecmp(argv[i], "Shared=", 7) == 0) {
			if (strcasecmp(&argv[i][7], "YES") == 0)
				job_msg.shared = 1;
			else if (strcasecmp(&argv[i][7], "NO") == 0)
				job_msg.shared = 0;
			else
				job_msg.shared = 
					(uint16_t) strtol(&argv[i][7], 
							(char **) NULL, 10);
			update_cnt++;
		}
		else if (strncasecmp(argv[i], "Contiguous=", 11) == 0) {
			if (strcasecmp(&argv[i][11], "YES") == 0)
				job_msg.contiguous = 1;
			else if (strcasecmp(&argv[i][11], "NO") == 0)
				job_msg.contiguous = 0;
			else
				job_msg.contiguous = 
					(uint16_t) strtol(&argv[i][11], 
							(char **) NULL, 10);
			update_cnt++;
		}
		else if (strncasecmp(argv[i], "ReqNodeList=", 12) == 0) {
			job_msg.req_nodes = &argv[i][12];
			update_cnt++;
		}
		else if (strncasecmp(argv[i], "Features=", 9) == 0) {
			job_msg.features = &argv[i][9];
			update_cnt++;
		}
		else if (strncasecmp(argv[i], "Account=", 8) == 0) {
			job_msg.account = &argv[i][8];
			update_cnt++;
		}
		else if (strncasecmp(argv[i], "Dependency=", 11) == 0) {
			job_msg.dependency =
				(uint32_t) strtol(&argv[i][11],
					(char **) NULL, 10);
			update_cnt++;
		}
#ifdef HAVE_BG
		else if (strncasecmp(argv[i], "Geometry=", 9) == 0) {
			char* token, *delimiter = ",x", *next_ptr;
			int j, rc = 0;
			uint16_t geo[SYSTEM_DIMENSIONS];
			char* geometry_tmp = xstrdup(&argv[i][9]);
			char* original_ptr = geometry_tmp;
			token = strtok_r(geometry_tmp, delimiter, &next_ptr);
			for (j=0; j<SYSTEM_DIMENSIONS; j++) {
				if (token == NULL) {
					error("insufficient dimensions in "
						"Geometry");
					rc = -1;
					break;
				}
				geo[j] = (uint16_t) atoi(token);
				if (geo[j] <= 0) {
					error("invalid --geometry argument");
					rc = -1;
					break;
				}
				geometry_tmp = next_ptr;
				token = strtok_r(geometry_tmp, delimiter, 
					&next_ptr);
			}
			if (token != NULL) {
				error("too many dimensions in Geometry");
				rc = -1;
			}

			if (original_ptr)
				xfree(original_ptr);
			if (rc != 0) {
				for (j=0; j<SYSTEM_DIMENSIONS; j++)
					geo[j] = (uint16_t) NO_VAL;
				exit_code = 1;
			} else
				update_cnt++;
			select_g_set_jobinfo(job_msg.select_jobinfo,
					     SELECT_DATA_GEOMETRY,
					     (void *) &geo);
		}

		else if (strncasecmp(argv[i], "Rotate=", 7) == 0) {
			uint16_t rotate;
			if (strcasecmp(&argv[i][7], "yes") == 0)
				rotate = 1;
			else if (strcasecmp(&argv[i][7], "no") == 0)
				rotate = 0;
			else
				rotate = (uint16_t) strtol(&argv[i][7], 
							   (char **) NULL, 10);
			select_g_set_jobinfo(job_msg.select_jobinfo,
					     SELECT_DATA_ROTATE,
					     (void *) &rotate);
			update_cnt++;
		}
		else if (strncasecmp(argv[i], "Connection=", 11) == 0) {
			uint16_t conn_type;
			if (strcasecmp(&argv[i][11], "torus") == 0)
				conn_type = SELECT_TORUS;
			else if (strcasecmp(&argv[i][11], "mesh") == 0)
				conn_type = SELECT_MESH;
			else if (strcasecmp(&argv[i][11], "nav") == 0)
				conn_type = SELECT_NAV;
			else
				conn_type = 
					(uint16_t) strtol(&argv[i][11], 
							(char **) NULL, 10);
			select_g_set_jobinfo(job_msg.select_jobinfo,
					     SELECT_DATA_CONN_TYPE,
					     (void *) &conn_type);
			update_cnt++;
		}
#endif
		else if (strncasecmp(argv[i], "StartTime=", 10) == 0) {
			job_msg.begin_time = parse_time(&argv[i][10]);
			update_cnt++;
		}
		else {
			exit_code = 1;
			fprintf (stderr, "Invalid input: %s\n", argv[i]);
			fprintf (stderr, "Request aborted\n");
			return 0;
		}
	}

	if (update_cnt == 0) {
		exit_code = 1;
		fprintf (stderr, "No changes specified\n");
		return 0;
	}

	if (slurm_update_job(&job_msg))
		return slurm_get_errno ();
	else
		return 0;
}
