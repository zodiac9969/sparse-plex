classdef Properties < handle
% Defines various properties of a dictionary
% See
% - 2011_learned_dictionaries_for_sparse_image_representation_properties_and_results.pdf
% - 
    properties(Access=private)
        % The dictionary matrix
        Dict = []
        Gram = []
        AbsGram = []
        Frame = []
        SingularValues = []
        % Signal space dimension
        N
        % Number of atoms
        D
        % Coherence
        Coherence = []
    end

    methods
        function self = Properties(Dict)
            if isa(Dict, 'spx.dict.Operator')
                self.Dict = double(Dict);
            elseif ismatrix(Dict)
                self.Dict = Dict; 
            else
                error('Unsupported dictionary.');
            end
            [self.N, self.D] = size(self.Dict);
        end

        function result = gram_matrix(self)
            if isempty(self.Gram)
                d = self.Dict;
                self.Gram = d' * d;
            end
            result = self.Gram;
        end

        function result = abs_gram_matrix(self)
            if isempty(self.AbsGram)
                g = self.gram_matrix();
                self.AbsGram = abs(g);
            end
            result = self.AbsGram;
        end

        function result = frame_operator(self)
            if isempty(self.Frame)
                d = self.Dict;
                self.Frame = d * d';
            end
            result = self.Frame;
        end

        function result = singular_values(self)
            if isempty(self.SingularValues)
                [U, S, V] = svd(self.Dict);
                self.SingularValues = diag(S)';
            end
            result = self.SingularValues;
        end

        function result = gram_eigen_values(self)
            % Returns the eigen values of the Gram matrix
            % They are same for frame operator
            % sum = D
            s = self.singular_values();
            result = s.^2;
        end

        function result = lower_frame_bound(self)
            lambda = self.gram_eigen_values();
            result = lambda(end);
        end

        function result = upper_frame_bound(self)
            lambda = self.gram_eigen_values();
            result = lambda(1);
        end

        function result = coherence(self)
            % Returns the coherence of the dictionary
            if isempty(self.Coherence)
                result = self.coherence_with_index();
            end
            result = self.Coherence;
        end

        function [ mu, i, j, absG ] = coherence_with_index(self)
            absG = self.abs_gram_matrix();
            absG(logical(eye(size(absG)))) = 0;
            [mu, index] = max(absG(:));
            self.Coherence = mu;
            [i, j] = ind2sub(size(absG), index);
            % Make sure that column numbers are reported in increasing order
            if i > j
                t = i;
                i = j;
                j = t;
            end
        end
    end
end
