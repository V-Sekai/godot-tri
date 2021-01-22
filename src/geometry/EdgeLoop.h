#pragma once

#include "g3types.h"
#include "DMesh3.h"

namespace g3
{
	// find the vtx that is the same in both ev0 and ev1
	static int find_shared_edge_v(const Index2i& ev0, const Index2i ev1)
	{
		if (ev0.x() == ev1.x()) {
			return ev0.x();
		}
		else if (ev0.x() == ev1.y()) {
			return ev0.x();
		}
		else if (ev0.y() == ev1.x()) {
			return ev0.y();
		}
		else if (ev0.y() == ev1.y()) {
			return ev0.y();
		}
		else {
			return DMesh3::InvalidID;
		}
	}

	static int find_edge_other_v(Index2i ev, int v)
	{
		if (ev.x() == v) {
			return ev.y();
		} else if (ev.y() == v) {
			return ev.x();
		} else {
			return DMesh3::InvalidID;
		}
	}


	class EdgeLoop;
	typedef std::shared_ptr<EdgeLoop> EdgeLoopPtr;

	/// <summary>
	/// Sequential set of vertices/edges in a mesh, that form a closed loop.
	/// 
	/// If all you have are the vertices, use EdgeLoop.VertexLoopToEdgeLoop() to construct an EdgeLoop
	/// </summary>
	class EdgeLoop
	{
		DMesh3Ptr Mesh;

		std::vector<int> Vertices;
		std::vector<int> Edges;
		std::vector<int> BowtieVertices;        // this may not be initialized!

	public:

		EdgeLoop(DMesh3Ptr mesh)
		{
			Mesh = mesh;
		}
		EdgeLoop(DMesh3Ptr mesh, std::vector<int> vertices, std::vector<int> edges)
		{
			Mesh = mesh;
			Vertices = vertices;
			Edges = edges;
		}

		EdgeLoop(EdgeLoopPtr copy)
		{
			Mesh = copy->Mesh;
			Vertices = copy->Vertices;
			Edges = copy->Edges;
			if (copy->BowtieVertices.size()) {
				BowtieVertices = copy->BowtieVertices;
			}
		}


		/// <summary>
		/// construct EdgeLoop from a list of edges of mesh
		/// </summary>
		EdgeLoopPtr FromEdges(DMesh3Ptr mesh, std::list<int> edges)
		{
			Edges.clear();
			for (int edge : edges) {
				Edges.push_back(edge);
			}

			std::vector<int> Vertices;
			Vertices.resize(Edges.size());
			Index2i start_ev = mesh->GetEdgeV(Edges[0]);
			Index2i prev_ev = start_ev;
			for (int i = 1; i < Edges.size(); ++i) {
				Index2i next_ev = mesh->GetEdgeV(Edges[i % Edges.size()]);
				Vertices[i] = find_shared_edge_v(prev_ev, next_ev);
				prev_ev = next_ev;
			}
			Vertices[0] = find_edge_other_v(start_ev, Vertices[1]);
			return std::make_shared<EdgeLoop>(mesh, Vertices, Edges);
		}


		/// <summary>
		/// construct EdgeLoop from a list of vertices of mesh
		/// </summary>
		EdgeLoopPtr FromVertices(DMesh3Ptr mesh, std::list<int> vertices)
		{
			int NV = vertices.size();
			std::vector<int> Vertices;
			Vertices.clear();
			for (int vertex : vertices) {
				Vertices.push_back(vertex);
			}
			int NE = NV;
			std::vector<int> Edges;
			Edges.resize(NE);
			for (int i = 0; i < NE; ++i) {
				Edges[i] = mesh->FindEdge(Vertices[i], Vertices[(i + 1) % NE]);
				if (Edges[i] == DMesh3::InvalidID) {
					throw std::logic_error("EdgeLoop.FromVertices: vertices are not connected by edge!");
				}
			}
			return std::make_shared<EdgeLoop>(mesh, Vertices, Edges);
		}

		/// <summary>
		/// construct EdgeLoop from a list of vertices of mesh
		/// if loop is a boundary edge, we can correct orientation if requested
		/// </summary>
		EdgeLoopPtr FromVertices(DMesh3Ptr mesh, std::list<int> vertices, bool bAutoOrient = true)
		{
			std::vector<int> Vertices;
			for (int vertex : vertices) {
				Vertices.push_back(vertex);
			}

			if (bAutoOrient) {
				int a = Vertices[0], b = Vertices[1];
				int eid = mesh->FindEdge(a, b);
				if (mesh->IsBoundaryEdge(eid)) {
					Index2i ev = mesh->GetOrientedBoundaryEdgeV(eid);
					if (ev.x() == b && ev.y() == a) {
						std::reverse(Vertices.begin(), Vertices.end());
					}
				}
			}

			std::vector<int> Edges;
			Edges.resize(Vertices.size());
			for (int i = 0; i < Edges.size(); ++i) {
				int a = Vertices[i], b = Vertices[(i + 1) % Vertices.size()];
				Edges[i] = mesh->FindEdge(a, b);
				if (Edges[i] == DMesh3::InvalidID)
					throw std::logic_error("EdgeLoop.FromVertices: invalid edge [" + a + ',' + b + ']');
			}

			return std::make_shared<EdgeLoop>(mesh, Vertices, Edges);
		}


		int VertexCount() {
			return Vertices.size();
		}
		int EdgeCount() {
			return Edges.size();
		}

		Vector3d GetVertex(int i) {
			return Mesh->GetVertex(Vertices[i]);
		}


		AxisAlignedBox3d GetBounds()
		{
			AxisAlignedBox3d box;
			for (int i = 0; i < Vertices.size(); ++i) {
				box.Contain(Mesh->GetVertex(Vertices[i]));
			}
			return box;
		}


		//DCurve3 ToCurve(DMesh3Ptr sourceMesh)
		//{
		//	if (sourceMesh == null)
		//		sourceMesh = Mesh;
		//	DCurve3 curve = MeshUtil.ExtractLoopV(sourceMesh, Vertices);
		//	curve.Closed = true;
		//	return curve;
		//}


		//	/// <summary>
		//	/// if this is a border edge-loop, we can check that it is oriented correctly, and
		//	/// if not, reverse it.
		//	/// Returns true if we reversed orientation.
		//	/// </summary>
		//	public bool CorrectOrientation()
		//	{
		//		int a = Vertices[0], b = Vertices[1];
		//		int eid = Mesh.FindEdge(a, b);
		//		if (Mesh.IsBoundaryEdge(eid)) {
		//			Index2i ev = Mesh.GetOrientedBoundaryEdgeV(eid);
		//			if (ev.a == b && ev.b == a) {
		//				Reverse();
		//				return true;
		//			}
		//		}
		//		return false;
		//	}


		//	public void Reverse()
		//	{
		//		Array.Reverse(Vertices);
		//		Array.Reverse(Edges);
		//	}


		//	/// <summary>
		//	/// check if all edges of this loop are internal edges (ie none on boundary)
		//	/// </summary>
		//	/// <returns></returns>
		//	public bool IsInternalLoop()
		//	{
		//		int NV = Vertices.Length;
		//		for (int i = 0; i < NV; ++i) {
		//			int eid = Mesh.FindEdge(Vertices[i], Vertices[(i + 1) % NV]);
		//			Debug.Assert(eid != DMesh3.InvalidID);
		//			if (Mesh.IsBoundaryEdge(eid))
		//				return false;
		//		}
		//		return true;
		//	}


		//	/// <summary>
		//	/// Check if all edges of this loop are boundary edges.
		//	/// If testMesh != null, will check that mesh instead of internal Mesh
		//	/// </summary>
		//	public bool IsBoundaryLoop(DMesh3 testMesh = null)
		//	{
		//		DMesh3 useMesh = (testMesh != null) ? testMesh : Mesh;

		//		int NV = Vertices.Length;
		//		for (int i = 0; i < NV; ++i) {
		//			int eid = useMesh.FindEdge(Vertices[i], Vertices[(i + 1) % NV]);
		//			Debug.Assert(eid != DMesh3.InvalidID);
		//			if (useMesh.IsBoundaryEdge(eid) == false)
		//				return false;
		//		}
		//		return true;
		//	}


		//	/// <summary>
		//	/// find index of vertex vID in Vertices list, or -1 if not found
		//	/// </summary>
		//	public int FindVertexIndex(int vID)
		//	{
		//		int N = Vertices.Length;
		//		for (int i = 0; i < N; ++i) {
		//			if (Vertices[i] == vID)
		//				return i;
		//		}
		//		return -1;
		//	}

		//	/// <summary>
		//	/// find index of vertices of loop that is closest to point v
		//	/// </summary>
		//	public int FindNearestVertex(Vector3d v)
		//	{
		//		int iNear = -1;
		//		double fNearSqr = double.MaxValue;
		//		int N = Vertices.Length;
		//		for (int i = 0; i < N; ++i) {
		//			Vector3d lv = Mesh.GetVertex(Vertices[i]);
		//			double d2 = v.DistanceSquared(lv);
		//			if (d2 < fNearSqr) {
		//				fNearSqr = d2;
		//				iNear = i;
		//			}
		//		}
		//		return iNear;
		//	}

		//	// count # of vertices in loop that are within tol of v
		//	// final param returns last encountered index within tolerance, or -1 if return is 0
		//	public int CountWithinTolerance(Vector3d v, double tol, out int last_in_tol)
		//	{
		//		last_in_tol = -1;
		//		int count = 0;
		//		int N = Vertices.Length;
		//		for (int i = 0; i < N; ++i) {
		//			Vector3d lv = Mesh.GetVertex(Vertices[i]);
		//			if (v.Distance(lv) < tol) {
		//				count++;
		//				last_in_tol = i;
		//			}
		//		}
		//		return count;
		//	}


		//	// Check if Loop2 is the same set of positions on another mesh.
		//	// Does not require the indexing to be the same
		//	// Currently doesn't handle loop-reversal
		//	public bool IsSameLoop(EdgeLoop Loop2, bool bReverse2 = false, double tolerance = MathUtil.ZeroTolerance)
		//	{
		//		// find a duplicate starting vertex
		//		int N = Vertices.Length;
		//		int N2 = Loop2.Vertices.Length;
		//		if (N != N2)
		//			return false;

		//		DMesh3 Mesh2 = Loop2.Mesh;

		//		int start_i = 0, start_j = -1;

		//		// try to find a unique same-vertex on each loop. Do not
		//		// use vertices that have duplicate positions.
		//		bool bFoundGoodStart = false;
		//		while (!bFoundGoodStart && start_i < N) {
		//			Vector3d start_v = Mesh.GetVertex(start_i);
		//			int count = Loop2.CountWithinTolerance(start_v, tolerance, out start_j);
		//			if (count == 1)
		//				bFoundGoodStart = true;
		//			else
		//				start_i++;
		//		}
		//		if (!bFoundGoodStart)
		//			return false;       // no within-tolerance duplicate vtx to start at

		//		for (int ii = 0; ii < N; ++ii) {
		//			int i = (start_i + ii) % N;
		//			int j = (bReverse2) ?
		//				MathUtil.WrapSignedIndex(start_j - ii, N2)
		//				: (start_j + ii) % N2;
		//			Vector3d v = Mesh.GetVertex(Vertices[i]);
		//			Vector3d v2 = Mesh2.GetVertex(Loop2.Vertices[j]);
		//			if (v.Distance(v2) > tolerance)
		//				return false;
		//		}

		//		return true;
		//	}



		//	/// <summary>
		//	/// stores vertices [starti, starti+1, ... starti+count-1] in span, and returns span, or null if invalid range
		//	/// </summary>
		//	public int[] GetVertexSpan(int starti, int count, int[] span, bool reverse = false)
		//	{
		//		int N = Vertices.Length;
		//		if (starti < 0 || starti >= N || count > N - 1)
		//			return null;
		//		if (reverse) {
		//			for (int k = 0; k < count; ++k)
		//				span[count - k - 1] = Vertices[(starti + k) % N];
		//		}
		//		else {
		//			for (int k = 0; k < count; ++k)
		//				span[k] = Vertices[(starti + k) % N];
		//		}
		//		return span;
		//	}




		//	/// <summary>
		//	/// Exhaustively check that verts and edges of this EdgeLoop are consistent. Not for production use.
		//	/// </summary>
		//	public bool CheckValidity(FailMode eFailMode = FailMode.Throw)
		//	{
		//		bool is_ok = true;
		//		Action<bool> CheckOrFailF = (b) = > { is_ok = is_ok && b; };
		//		if (eFailMode == FailMode.DebugAssert) {
		//			CheckOrFailF = (b) = > { Debug.Assert(b); is_ok = is_ok && b; };
		//		}
		//		else if (eFailMode == FailMode.gDevAssert) {
		//			CheckOrFailF = (b) = > { Util.gDevAssert(b); is_ok = is_ok && b; };
		//		}
		//		else if (eFailMode == FailMode.Throw) {
		//			CheckOrFailF = (b) = > { if (b == false) throw new Exception("EdgeLoop.CheckValidity: check failed"); };
		//		}

		//		CheckOrFailF(Vertices.Length == Edges.Length);
		//		for (int ei = 0; ei < Edges.Length; ++ei) {
		//			Index2i ev = Mesh.GetEdgeV(Edges[ei]);
		//			CheckOrFailF(Mesh.IsVertex(ev.a));
		//			CheckOrFailF(Mesh.IsVertex(ev.b));
		//			CheckOrFailF(Mesh.FindEdge(ev.a, ev.b) != DMesh3.InvalidID);
		//			CheckOrFailF(Vertices[ei] == ev.a || Vertices[ei] == ev.b);
		//			CheckOrFailF(Vertices[(ei + 1) % Edges.Length] == ev.a || Vertices[(ei + 1) % Edges.Length] == ev.b);
		//		}
		//		for (int vi = 0; vi < Vertices.Length; ++vi) {
		//			int a = Vertices[vi], b = Vertices[(vi + 1) % Vertices.Length];
		//			CheckOrFailF(Mesh.IsVertex(a));
		//			CheckOrFailF(Mesh.IsVertex(b));
		//			CheckOrFailF(Mesh.FindEdge(a, b) != DMesh3.InvalidID);
		//			int n = 0, edge_before_b = Edges[vi], edge_after_b = Edges[(vi + 1) % Vertices.Length];
		//			foreach(int nbr_e in Mesh.VtxEdgesItr(b)) {
		//				if (nbr_e == edge_before_b || nbr_e == edge_after_b)
		//					n++;
		//			}
		//			CheckOrFailF(n == 2);
		//		}
		//		return is_ok;
		//	}




		/// <summary>
		/// Convert a vertex loop to an edge loop. This should be somewhere else...
		/// </summary>
		std::vector<int> VertexLoopToEdgeLoop(DMesh3Ptr mesh, std::vector<int> vertex_loop)
		{
			int NV = vertex_loop.size();
			std::vector<int> edges;
			edges.resize(NV);
			for (int i = 0; i < NV; ++i) {
				int v0 = vertex_loop[i];
				int v1 = vertex_loop[(i + 1) % NV];
				edges[i] = mesh->FindEdge(v0, v1);
			}
			return edges;
		}
	};
}