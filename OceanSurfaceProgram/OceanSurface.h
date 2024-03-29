#pragma once

#include "structure_utils.h"
#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include <glm\geometric.hpp>
#include "MyFFT.h"
#include "gl_utils.h"
#include <SOIL.h>

// class for ocean surface definition

class OceanSurface {
	private:
		int N, M; // grid size for wave height field (should be power of two both)
		float L_x, L_z; // real world dimensions for wave height field (in meters)
		surface_vertex *grid; // each point in grid consists of 3d-world coordinates (x, y, z)
		vertex_2d *init_positions; // initial positions for all grid points on plane OXZ
		tex_img_coord *grid_tex_cord;
		complex_number *h_t0, *h_t0_cc; // array of precomputed random complex amplitudes
		complex_number *h_fft; // fft-grid for height field
		complex_number *dx_fft; // x-displacement for each grid point
		complex_number *dz_fft; // z-displacement for each grid point
		complex_number *gradient_x; // x-component of gradient of 'h_fft' at each grid point
		complex_number *gradient_z; // z-component of gradient of 'h_fft' at each grid point
		float lambda; // numeric constant for calculating displacement
		float A; // numeric constant for wave height in phillips spectrum
		glm::vec2 wind; // vector of wind direction
		float g; // gravitationl constant
		GLuint *indices; // indices for index drawing of grid
		GLuint *triangle_indices; // indices for drawing of triangles
		int num_indices; // number of indices
		int num_triangle_indices; // indices of triangles
		GLuint indices_vbo, triangle_indices_vbo, points_vbo, vao; // vbo for indices and vertices, vao for vertices
		void prepare_for_pipeline();
		void calc_binary_reverse(int bits);
		int *binary_reverse;
		cFFT *fft;
		MyFFT *myFFT;
		GLuint fft_comp_program; // FFT in compute shader

		// variables of update height map compute program
		GLuint upd_height_program, total_time_location;
		GLuint texture_H_t0, texture_H_t0_cc;
		GLuint texture_Dx, texture_Dz, texture_Gradx, texture_Gradz;

		GLuint texture_H_t, texture_H_fft_t_row, texture_H_fft_t_col, fft_column_location;
		GLuint tex_dx_fft_row, tex_dx_fft;
		GLuint tex_dz_fft_row, tex_dz_fft;
		GLuint tex_gradx_fft_row, tex_gradx_fft;
		GLuint tex_gradz_fft_row, tex_gradz_fft;
		GLuint rev_ind_buffer, img_coord_vbo;

		bool lines;
		bool gpu;

		GLuint tex_Fresnel, tex_ReflectCube;
		float D3DXFresnelTerm(float costheta, float refractionindex);

	public:
		OceanSurface(int N, int M, float L_x, float L_z, float A, glm::vec2 _wind, float g,
			bool _lines, bool _gpu); // constructor
		~OceanSurface(); // destructor
		// here and further n = n' and m = m' (equally to formulas for n'=0..N-1, m'=0..M-1)
		float phillips_spectrum(int n, int m);
		complex_number h_t_0(int n, int m);
		float dispersion_relation(int n, int m);
		complex_number h_t(int n, int m, float t);
		float h(float x, float z, float t);
		void updateOceanSlow(float t);
		void updateOcean(float t);
		void render();
};
