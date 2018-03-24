function n = getSeqEntityBits(SeqEntity)
    %GETSEQENTITYBITS returns the number of bits required to store the SeqEntity
    
    % SeqEntity
    n = 32 + 12 + 12;  % SeqHeader
    for i = 1:length(SeqEntity.GoPEntityArray)
        n = n + getGoPEntityBits(SeqEntity.GoPEntityArray{i});
    end
    n = n + 32;  % SeqEnd
end

function n = getGoPEntityBits(GoPEntity)
    n = 32;  % GoPHeader
    for i = 1:length(GoPEntity.PicSliceEntityArray)
        n = n + getPicSliceEntityBits(GoPEntity.PicSliceEntityArray{i});
    end
end

function n = getPicSliceEntityBits(PicSliceEntity)
    n = 32 + 10 + 3 + 32 + 5;  % PicSliceHeader
    for i = 1:length(PicSliceEntity.MBEntityArray)
        n = n + getMBEntityBits(PicSliceEntity.MBEntityArray{i});
    end
end

function n = getMBEntityBits(MBEntity)
    n = 3 + 5;  % MBHeader
    if strcmp(MBEntity.MBHeader.macroblock_type, 'P')  % Motion Vectors
        n = n + 2 * 16;  % 2 components, 17  bits each
    elseif strcmp(MBEntity.MBHeader.macroblock_type, 'B')
        n = n + 4 * 16;  % 4 components, 17  bits each
    end
    for i = 1:length(MBEntity.BlockEntityArray)  % BlockEntityArray
        n = n + getBlockEntityBits(MBEntity.BlockEntityArray{i});
    end
end

function n = getBlockEntityBits(BlockEntity)
    n = 0;
    for i = 1:length(BlockEntity.VLCodes)  % VLCodes
        n = n + length(BlockEntity.VLCodes);
    end
end
